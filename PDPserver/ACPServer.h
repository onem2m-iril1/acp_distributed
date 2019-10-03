#ifndef _MAF_SERVER_H
#define _MAF_SERVER_H

//	File Name:		MAF_Server.cpp
//	File Purpose:	This file contains detailed implementation of a Server application 
//					which is responsible to authenticate two end to end devices
//	Date & Time:	June 08, 2019 | 19:09 PM
//	Author:			Bilal Imran

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <regex>
#include <string>
#include <stdlib.h>
#include <sys/time.h>
#include <netinet/in.h>

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/hmac.h>
#include "PDPserverFiles/PDP/acpd_pdp.h"

#include "MbedJSONValue.h"
#include "sqlite3.h"

using namespace std;

/* DTLS Control Parameter 
 * <DTLS-with-PSK> USE_PSK
 * <DTLS-with-CERT> USE_CERT
 * */
//#define USE_CERT 			
#define USE_PSK

#define MSGLEN				4096
#define MAX_PACKET_SIZE			1500
#define PSK_KEY_LENGTH			5 			
#define COOKIE_SECRET_LENGTH 		8
#define MaxRows				3
#define MaxColumns			3
#define MaxUpperBytes			16						// 16 Bytes or (32 hex characters)
#define MaxLowerBytes			32						// 32 Bytes or (64 hex characters)
#define MAXIDLength             	20

#define Client_REG_SUCCESS		2001	

const char *sqlite_name		=	"GBA-Server.db";
const char *keymatexportlabel 	= 	"oneM2M-Key-Export";
const char *psk_hint 		= 	"bridge";
int uniPortNo 			= 	10001;
int keymatexportlen		= 	48; 					// 48 Bytes or (96 hex characters)
const char* ipAddress 		= 	"192.168.0.62";

// MAF-Client Registation Parameters
#define USE_KEY_FOR_SAEF		11
#define CREATE_OP			1
#define MAF_CLIENT_TYPE			3
#define MAF_CLIENT_RSC 			2001
#define Client_KEY_REG_SUCCESS		2001
#define CLIENT_KEY_RET_SUCCESS		2000

std::string		MAF_CLIENT_RQI;
std::string 		RANDOM_ID;
std::string		MAF_CLIENT_KEY_RQI;
std::string 		MAF_CLIENT_RET;
std::string		MAF_CLIENT_TO;
std::string		MAF_CLIENT_KEY_LB;
std::string		MAF_CLIENT_KEY_ET;
std::string		MAF_CLIENT_KEY_TGIS;
std::string		MAF_CLIENT_KEY_ID;
std::string		MAF_CLIENT_KEY_VALUE;
std::string 		MAF_CLIENT_CR;

char 			MAF_CLIENT_RI[MAXIDLength];
char 			strtemp[8];
int 			MAF_CLIENT_KEY_SUID;
bool 			idAvailable;
time_t 			t;

// Global Variables
const char* 		caFILE 	= "/home/pi/Desktop/Trusted_CA.crt";
const char* 		caKEY; 
const char* 		srvFILE = "/home/pi/Desktop/mncse_srv.crt";
const char* 		srvKEY	= "/home/pi/Desktop/mncse_srv.key";

//MAFServerDataBase 	dbclient;
sqlite3			*sqlite_client;

char* 			newrow[5];
char*			zErrMsg	= 0;
char*			sqlite_command;

MbedJSONValue 		MSG, MSG_1;

char*	 	 	maf_json_buf;
char* 			serverbuf;
char* 			send_buf;
char*		 	srvrespbuf;
char 			recv_buf[MAX_PACKET_SIZE];
char 			socketBUFF[MSGLEN];
unsigned char 		ExportKeyIdentifier[MaxUpperBytes];
unsigned char 		ExportKeyValue[MaxLowerBytes];
unsigned char* 		exportedkeymat;
std::string 		maf_string_buf;
int 				op, ty;

int 				fd;
int 				reuse = 1;

SSL_CTX *ctx;
SSL *ssl;

BIO *bio;
BIO *for_reading;
BIO *for_writing;

struct timeval timeout;
struct pass_info *info;

int cookie_initialized;
unsigned char cookie_secret[COOKIE_SECRET_LENGTH];


/* Psk Look-UP Table (Psk-LUPT)
 * 'Row' represents the client entries
 * 'Column' represents the client's info such as;
 * <Column00> Device ID
 * <Column01> Psk_Identity
 * <Column02> Psk Value (in Hex) */
const char* Psk_LuPT[MaxRows][MaxColumns] = {
{"ADN-AE-01", "AE123-LOCK@in.provider.com", 	"0102030405"},
{"ADN-AE-02", "AE456-LOCK@in.provider.com", 	"0504030201"},
{"MN-CSE-01", "CSE123-Gateway@in.provider.com", "0101020305"}
}; 

static struct       sockaddr_in cliAddr;    /* the client's address */
static struct       sockaddr_in servAddr;   /* our server's address */

union {
		struct sockaddr_storage ss;
		struct sockaddr_in s4;
	} peer_addr;

void* ThreadControl(void*);

typedef struct {
    int activefd;
    int size;
    unsigned char b[MSGLEN];
    SSL *ssl;
    union {
	struct sockaddr_storage ss;
	struct sockaddr_in s4;
    } cliAddr;
} threadArgs;

int           on = 1;
int           res = 1;
int           bytesRcvd = 0;
socklen_t     cliLen;
socklen_t     len = sizeof(on);
unsigned char buf[MSGLEN];      /* watch for incoming messages */
/* variables needed for threading */
threadArgs* args;
pthread_t threadid;
pthread_mutex_t lock;

//	Function Declarations
const char* FindPsk (const char* psk_id);
static unsigned int psk_server_cb(SSL *ssl, const char *identity,
                                  unsigned char *psk,
                                  unsigned int max_psk_len);
int dtls_verify_callback (int ok, X509_STORE_CTX *ctx); 
static int callback (void *NotUsed, int args, char**argv, char **azColName);
static int generate_cookie(SSL *ssl, unsigned char *cookie, unsigned int *cookie_len);
static int verify_cookie(SSL *ssl, const unsigned char *cookie, unsigned int cookie_len);
int MAFServerUDPPortOpen();
int MAFServerDTLSInitialize();
int MAFServerDTLSListen();
int MAFServerDTLSHandshake();
int MAFServerSend();
int MAFServerRecieve();
int MAFServerStart();
int Process_MSG();

#endif
