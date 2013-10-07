#!/bin/bash


INSTALL='apt-get -y install'
WEBDIR=labrea
APACHE=apache2
SERVICE=/etc/init.d/
#SERVICE='service '

# install libpcap
echo "Installing libpcap-dev"
${INSTALL} libpcap-dev && echo "OK"

# build LaBrea6 source code
echo "Building LaBrea6 from source"
TMP=`pwd`
cd ./src
./build.sh && echo "OK"
cd ${TMP}

# install mysql server
# nutno zadat root heslo
echo "Installing the mysql server. You'll be prompted for root password!"
${INSTALL} mysql-server && echo "OK"

# setup the database table and user
echo "Running labrea.sql to create database, user and table"
mysql -u root -p < ./scripts/labrea.sql && echo "OK"

# install apache2 php php-mysql libs
echo "Installing php5 apache2 and php5-mysql libs"
${INSTALL} php5-mysql php5 apache2 libapache2-mod-php5 && echo "OK"

#place php skripts into the web server
echo "Creating web pages on the server"
test -d /var/www/${WEBDIR} || mkdir /var/www/${WEBDIR}
echo "Copying web pages to the server root"
cp -r ./web/* /var/www/${WEBDIR}/ && echo "OK"
# reaload apache
echo "Restarting apache web server"
${SERVICE}${APACHE} restart && echo "OK"

