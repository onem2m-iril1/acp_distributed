# ACP_Distributed
OneM2M compliant distributed ACP (Access Control Policy) with PHP based web interface.

This is the PDP server containing the three policy points i.e. PIP, PRP and PDP as described in TR-0016-Authorization_Architecture_and_Access_Control_Policy-V2_0_0.pdf at http://www.onem2m.org/images/files/deliverables/Release2/TR-0016-Authorization_Architecture_and_Access_Control_Policy-V2_0_0.pdf

- Policy Decision Point (PDP):
PDP interacts with the PRP and PIP to get applicable authorization polices and attributes needed to evaluate authorization policies respectively, and then evaluates access requests using authorization policies to render an access control decision. The PDP is located in the Authorization service.

- Policy Retrieval Point (PRP):
PRP obtains applicable authorization policies according to an access control decision request. These applicable policies should be combined in order to get a finial access control decision. The PRP is located in the Authorization service.

- Policy Information Point (PIP):
PIP provides attributes that are needed to evaluate authorization policies, for example the IP address of the requester, creation time of the resource, current time or location information of the requester. The PIP is located in the Authorization service.


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

Apache2, SQLite3, PHP7.3 on Raspberry Pi 3B+

Run this command to check if you already have apache2 installed
```
$ apache2 -v
```
If not installed then install apache2
```
$ sudo apt-get install apache2
```

Run this command to check if you already have PHP7.3 installed
```
$ php -v
```
If not installed then install PHP7.3
```
$ sudo apt-get install php7.3
```
Install required packages
```
$ sudo apt install php7.3-cli php7.3-common php7.3-curl php7.3-mbstring php7.3-mysql php7.3-xml
```
Set appropriate directory permissions
```
$ sudo chown www-data:www-data /var/www
$ sudo chmod 775 /var/www #permission to write to this directory
$ sudo usermod -a -G www-data pi #add the “Pi” user to the “www-data” group
```
Install SQLite3 module for PHP. This package provides the SQLite3 module(s) for PHP.
```
$ sudo apt-get install php7.3-sqlite3
```
Make sure the correct package is installed and apache restarted after that using this command:
```
$ sudo systemctl restart apache2
```
If that didn't work:
Then, remove the semicolon before these lines or correct the name for the extension in php.ini to:
```
$ extension=sqlite3.so
$ extension=pdo_sqlite.so
```
And restart apache again:
```
$ sudo systemctl restart apache2
```
Install OpenSSL Library using the following command
```
$ sudo apt-get install libssl-dev
```

### Installing

- Copy the "PDPserver" folder in /home/pi/. 
- Copy the "html" folder in /var/www/
- Build and run the program in current directory using following commands 

```
$ cd PDPserver/
$ make
$ ./ACPserver
``` 


### System Architechture

![Alt text](SystemArchitechture.jpg?raw=true "System Architechture")

## Authors

* **Muhammad Rehan** - *Initial work* - [rehan199](https://github.com/rehan199)

## License

KICS UET

## Team Members
- Muhammad Rehan
- Muhammad Ahsan
- Bilal Imran
- Asim Tanwir
