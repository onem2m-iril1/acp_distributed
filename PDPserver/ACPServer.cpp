//	File Name:		MAF_Server.cpp
//	File Purpose:	This file contains detailed implementation of a Server application 
//					which is responsible to authenticate two end to end devices
//	Date & Time:	June 08, 2019 | 19:09 PM
//	Author:			Bilal Imran

#include "ACPServer.h"

int callback (void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

/* Psk-LUPT Associated Functions 
 * <Function01> Check if the called Psk_Identity is available inside LuPT, if so
 * then return the associated Psk Value */
const char* FindPsk (const char* psk_id) {
		int i = 0;
		while(i < MaxRows) {
			if (strcmp(Psk_LuPT[i][1], psk_id) != 0)
				i++;
			else
				return Psk_LuPT[i][2];
		}
		return "Not Found Psk-ID";
}

/* Psk Call-Back Functions */
unsigned int psk_server_cb(SSL *ssl, const char *identity,
                                  unsigned char *psk,
                                  unsigned int max_psk_len)
{
    long key_len = 0;
    unsigned char *key;

    if (identity == NULL) {
        printf("Error: client did not send PSK identity\n");
        // goto out_err;
        printf("Error in PSK server callback\n");
		return 0;
    }
   
   /* here we could lookup the given identity e.g. from a database */
    const char* psk_ret = FindPsk(identity);
    if ( strcmp(psk_ret, "Not Found Psk-ID") == 0 ) {
        printf("PSK warning: client identity not what we expected \n");
        // goto out_err;
        printf("Error in PSK server callback\n");
		return 0;
    } else
        printf("PSK client identity found\n");
   
    /* convert the PSK key to binary */
    key = OPENSSL_hexstr2buf(psk_ret, &key_len);
    if (key == NULL) {
        printf("Could not convert PSK key '%s' to buffer\n", psk_ret);
        return 0;
    }
    if (key_len > (int)max_psk_len) {
        printf("psk buffer of callback is too small (%d) for key (%ld)\n",
                   max_psk_len, key_len);
        OPENSSL_free(key);
        return 0;
    }

    memcpy(psk, key, key_len);
    OPENSSL_free(key);

    printf("fetched PSK len=%ld\n", key_len);
    return key_len;

/* out_err:
    printf("Error in PSK server callback\n");
    return 0; */
}

int dtls_verify_callback (int ok, X509_STORE_CTX *ctx) {
	/* This function should ask the user
	 * if he trusts the received certificate.
	 * Here we always trust.
	 */
	return 1;
}

int generate_cookie(SSL *ssl, unsigned char *cookie, unsigned int *cookie_len)
{
	unsigned char *buffer, result[EVP_MAX_MD_SIZE];
	unsigned int length = 0, resultlength;
	union {
		struct sockaddr_storage ss;
		struct sockaddr_in s4;
	} peer;

	/* Initialize a random secret */
		if (!RAND_bytes(cookie_secret, COOKIE_SECRET_LENGTH))
			{
			printf("error setting random cookie secret\n");
			return 0;
			}
	
	/* Read peer information */
	(void) BIO_dgram_get_peer(SSL_get_rbio(ssl), &peer);

	/* Create buffer with peer's address and port */
	length = 0;
	switch (peer.ss.ss_family) {
		case AF_INET:
			length += sizeof(struct in_addr);
			break;
		default:
			OPENSSL_assert(0);
			break;
	}
	length += sizeof(in_port_t);
	buffer = (unsigned char*) OPENSSL_malloc(length);

	if (buffer == NULL)
		{
		printf("out of memory\n");
		return 0;
		}

	switch (peer.ss.ss_family) {
		case AF_INET:
			memcpy(buffer,
				   &peer.s4.sin_port,
				   sizeof(in_port_t));
			memcpy(buffer + sizeof(peer.s4.sin_port),
				   &peer.s4.sin_addr,
				   sizeof(struct in_addr));
			break;
		default:
			OPENSSL_assert(0);
			break;
	}

	/* Calculate HMAC of buffer using the secret */
	HMAC(EVP_sha1(), (const void*) cookie_secret, COOKIE_SECRET_LENGTH,
		 (const unsigned char*) buffer, length, result, &resultlength);
	OPENSSL_free(buffer);

	memcpy(cookie, result, resultlength);
	*cookie_len = resultlength;

	return 1;
}

int verify_cookie(SSL *ssl, const unsigned char *cookie, unsigned int cookie_len)
{
	unsigned char *buffer, result[EVP_MAX_MD_SIZE];
	unsigned int length = 0, resultlength;
	union {
		struct sockaddr_storage ss;
		struct sockaddr_in s4;
	} peer;

	/* Read peer information */
	(void) BIO_dgram_get_peer(SSL_get_rbio(ssl), &peer);

	/* Create buffer with peer's address and port */
	length = 0;
	switch (peer.ss.ss_family) {
		case AF_INET:
			length += sizeof(struct in_addr);
			break;
		default:
			OPENSSL_assert(0);
			break;
	}
	length += sizeof(in_port_t);
	buffer = (unsigned char*) OPENSSL_malloc(length);

	if (buffer == NULL)
		{
		printf("out of memory\n");
		return 0;
		}

	switch (peer.ss.ss_family) {
		case AF_INET:
			memcpy(buffer,
				   &peer.s4.sin_port,
				   sizeof(in_port_t));
			memcpy(buffer + sizeof(in_port_t),
				   &peer.s4.sin_addr,
				   sizeof(struct in_addr));
			break;
		default:
			OPENSSL_assert(0);
			break;
	}

	/* Calculate HMAC of buffer using the secret */
	HMAC(EVP_sha1(), (const void*) cookie_secret, COOKIE_SECRET_LENGTH,
		 (const unsigned char*) buffer, length, result, &resultlength);
	OPENSSL_free(buffer);
	
	if (cookie_len == resultlength && memcmp(result, cookie, resultlength) == 0) {
			return 1;
	}
	
	return 0;
}

int MAFServerDTLSInitialize() {	

/* Module 01:
 * [Start] SSL Session Creation via Context _ctx */
	SSL_library_init();
	OpenSSL_add_ssl_algorithms();
	SSL_load_error_strings();
	ERR_load_BIO_strings();
	ctx = SSL_CTX_new(DTLSv1_2_server_method());

/* We accept all ciphers, including NULL.
 * Not recommended beyond testing and debugging */	
// SSL_CTX_set_cipher_list(ctx, "ECDHE-ECDSA-AES256-GCM-SHA384");
	
	SSL_CTX_set_cipher_list(ctx, "ALL:NULL:eNULL:aNULL");
	SSL_CTX_set_session_cache_mode(ctx, SSL_SESS_CACHE_OFF);

#ifdef USE_CERT	 
	if (!SSL_CTX_use_certificate_file(ctx, srvFILE, SSL_FILETYPE_PEM))
		printf("\nERROR: no certificate found!");
		
	if (!SSL_CTX_use_PrivateKey_file(ctx, srvKEY, SSL_FILETYPE_PEM))
		printf("\nERROR: no private key found!");

	if (!SSL_CTX_check_private_key (ctx))
		printf("\nERROR: invalid private key!");

	/* Client has to authenticate */
	SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, dtls_verify_callback);
	SSL_CTX_set_verify_depth(ctx, SSL_VERIFY_NONE);
#endif

#ifdef USE_PSK
	/* Use Psk & it's Psk-Hint */
	SSL_CTX_set_psk_server_callback(ctx, &psk_server_cb);
	SSL_CTX_use_psk_identity_hint(ctx, psk_hint);
#endif

	SSL_CTX_set_read_ahead(ctx, 1);
	SSL_CTX_set_cookie_generate_cb(ctx, generate_cookie);
	SSL_CTX_set_cookie_verify_cb(ctx, &verify_cookie);
// [END]
	printf("DTLS Initialised \n");
	return 0;
}

int MAFServerDTLSListen() {
	
	args = (threadArgs *) malloc(sizeof(threadArgs));	
	memset(&args->cliAddr, 0, sizeof(struct sockaddr_storage));
	
	/* Create BIO */
	bio = BIO_new_dgram(fd, BIO_NOCLOSE);

	/* Set and activate timeouts */
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	BIO_ctrl(bio, BIO_CTRL_DGRAM_SET_RECV_TIMEOUT, 0, &timeout);
	
	ssl = SSL_new(ctx);
	SSL_set_bio(ssl, bio, bio);
	SSL_set_options(ssl, SSL_OP_COOKIE_EXCHANGE);

	int dtls_ret;
	do {
		dtls_ret = DTLSv1_listen(ssl, (BIO_ADDR *) &args->cliAddr);
	} while (dtls_ret <= 0); 

	printf("DTLS Listened Successfully \n");
	return 0;
}

int MAFServerDTLSHandshake() {	
/* Finish handshake */
	int ret;
	do { 
		ret = SSL_accept(ssl);
	}
	while (ret <= 0);
	
	char* buf;
	if (ret < 0) {
		perror("SSL_accept");
		printf("%s\n", ERR_error_string(ERR_get_error(), buf));
		return ret;
	}
	
/* Set and activate timeouts */
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	BIO_ctrl(SSL_get_rbio(ssl), BIO_CTRL_DGRAM_SET_RECV_TIMEOUT, 0, &timeout);

	if (SSL_get_peer_certificate(ssl)) {
		printf ("------------------------------------------------------------\n");
		X509_NAME_print_ex_fp(stdout, X509_get_subject_name(SSL_get_peer_certificate(ssl)), 1, XN_FLAG_MULTILINE);
		printf("\n\n Cipher: %s", SSL_CIPHER_get_name(SSL_get_current_cipher(ssl)));
		printf ("\n------------------------------------------------------------\n\n");
	} 
	
	printf("DTLS Handshake Successfully \n");	
	return 0;
}

int MAFServerUDPPortOpen()
{
	if (uniPortNo == 0)
	{
		printf("error portNo undefined in UDPPort::open\n");
		return -1;
	}

	uint32_t ip = inet_addr(ipAddress);
	
	/*------ Create unicast socket --------*/
	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (fd < 0)
	{
		printf("fd1 error can't create unicast socket in UDPPort::open\n");
		return -1;
	}

	/* clear servAddr each loop */
    //memset((char *)&servAddr, 0, sizeof(servAddr));

	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(uniPortNo);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (::bind(fd, (sockaddr*) &servAddr, sizeof(servAddr)) < 0)
	{
		printf("fd1 error can't bind unicast socket in UDPPort::open\n");
		return -1;
	}

	printf("UDP Port Opened \n");
	return 0;
}

int MAFServerSend() {
	return SSL_write(ssl, send_buf, MAX_PACKET_SIZE);
}

int MAFServerRecieve() {
	return SSL_read(ssl, recv_buf, MAX_PACKET_SIZE);
}

int Process_MSG(SSL *ssl) {
	
	cout << "Msg from the Client:" << endl;
	serverbuf = recv_buf;

	string strTemp = serverbuf;
	std::cout << "MsgStart:" << serverbuf << endl;
	std::cout << "MsgEnd" << endl;

	if (acpd_pdp::getPEPrequest(strTemp)){
		maf_string_buf = "1000";
		cout << "ACP matched" << endl;
	}
	else{
		maf_string_buf = "4005";
		cout << "ACP NOT matched" << endl;		
	}
	return 0;
}

int MAFServerStart() {
	int rc;
	int on = 1;
	int i = 0;
	MAFServerUDPPortOpen();
	MAFServerDTLSInitialize();

	// initializing the lock mutex
	if (pthread_mutex_init(&lock, NULL) != 0) 
    { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    } 

    int res;
    while (1) {
        printf("Awaiting client connection on port %d\n", uniPortNo);
        memset(&threadid, 0, sizeof(threadid));
        args = (threadArgs *) malloc(sizeof(threadArgs));	
        memset(&socketBUFF, 0, sizeof(socketBUFF));
		memset(&cliAddr, 0, sizeof(cliAddr));
		cliLen = sizeof(cliAddr);
	    
	    do {
	    	bytesRcvd = (int)recvfrom(fd, (char*) socketBUFF, MSGLEN, 0,
    		        (struct sockaddr*) &cliAddr, &cliLen);
	    } while (bytesRcvd <= 0);

        if (bytesRcvd < 0) {
            printf("No clients in que, enter idle state\n");
            continue;
        }

        else if (bytesRcvd > 0) {

            /* put all the bytes from buf into args */
            memcpy(args->b, socketBUFF, MAX_PACKET_SIZE);

            args->size = bytesRcvd;

            if ((args->activefd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
                printf("Cannot create socket.\n");
                goto END;
            }

            res = setsockopt(args->activefd, SOL_SOCKET, SO_REUSEADDR, &on,
                    sizeof(on));

            if (res < 0) {
                printf("Setsockopt SO_REUSEADDR failed.\n");
                goto END;
            }

            #ifdef SO_REUSEPORT
                res = setsockopt(args->activefd, SOL_SOCKET, SO_REUSEPORT, &on,
                        sizeof(on));
                if (res < 0) {
                    printf("Setsockopt SO_REUSEPORT failed.\n");
                    goto END;    
                }
            #endif

            if (::bind(args->activefd, (const struct sockaddr *)&servAddr, 
            	sizeof(struct sockaddr_in)) != 0) {
                printf("Udp bind failed.\n");
                goto END;
            }   	
               
            if (::connect(args->activefd, (const struct sockaddr *)&cliAddr,
                        sizeof(cliAddr)) != 0) {
                printf("Udp connect failed.\n");
                goto END;
            }
        }
        else {
            /* else bytesRcvd = 0 */
            printf("Recvfrom failed.\n");
            goto END;
        }
        printf("Connected!\n");

        /* SPIN A THREAD HERE TO HANDLE "buff" and "reply/ack" */
        pthread_create(&threadid, NULL, ThreadControl, args);
        printf("control passed to ThreadControl.\n");

        // wait 2 sec
        sleep(2);
    }

END:
	close(fd);
	return -1;
}

void* ThreadControl(void* openSock)
{
    //pthread_detach(pthread_self());
    int on = 1;
    threadArgs* args = (threadArgs*)openSock;
    int                recvLen = 0;               
    int                activefd; 
    int                msgLen = args->size;        
    unsigned char      buff[MAX_PACKET_SIZE];              
    char               ack[] = "I hear you fashizzle!\n";
    SSL 	       *ssl;
    int                e;                      
    int 	       rc;
    memset(&args->cliAddr, 0, sizeof(struct sockaddr_storage));

    printf("SSL new Session created \n");
    if ((ssl = SSL_new(ctx)) == NULL) {
	printf("SSL_new error.\n");
	goto END;
    } 
	
    bio = BIO_new_dgram(args->activefd, BIO_NOCLOSE);

    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    BIO_ctrl(bio, BIO_CTRL_DGRAM_SET_RECV_TIMEOUT, 0, &timeout);
	
    SSL_set_bio(ssl, bio, bio);
    SSL_set_options(ssl, SSL_OP_COOKIE_EXCHANGE);

    int dtls_ret;
    do {
	dtls_ret = DTLSv1_listen(ssl, (BIO_ADDR *) &args->cliAddr);
    } while (dtls_ret <= 0); 
    
    printf("SSl handshake Started \n"); 
    int ret;
	do { 
		ret = SSL_accept(ssl);
	}
	while (ret <= 0);
	
	char* buf;
	if (ret < 0) {
		printf("%s\n", ERR_error_string(ERR_get_error(), buf));
		goto END;
	}

	printf("SSL Handshake is complete \n");
 	
RECIEVE_MODE:

	do {
		rc = SSL_read(ssl, recv_buf, MAX_PACKET_SIZE);
	} while(rc <= 0);
	
	pthread_mutex_lock(&lock);
	Process_MSG(ssl);
	pthread_mutex_unlock(&lock); 
	
	send_buf = (char*) maf_string_buf.c_str();
	if ((rc =  SSL_write(ssl, send_buf, strlen((const char*)send_buf))) <= 0)
		printf("MAFServer Couldn't send the Msg \n");
	else
		printf("Message Sent to Client [Status : OK]\n");

goto RECIEVE_MODE;

END:
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(activefd);
    free(openSock);                
    printf("Client left return to idle state\n");
    printf("Exiting thread.\n\n");
    pthread_exit(openSock); 
}

//	Main Function
int main (void) {
	MAFServerStart();
	return 0;
}
