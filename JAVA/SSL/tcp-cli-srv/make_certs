#!/bin/bash
STOREPASS="forgotten"
for ENT in server_J client1_J client2_J client3_J client4_J ; do
 rm -f ${ENT}.jks ${ENT}.pem
 echo -e "${ENT}\nDEI\nISEP\nPORTO\nPORTO\nPT\nyes" | keytool -genkey -v -alias ${ENT} -keyalg RSA -keysize 2048 \
	-validity 365 -keystore ${ENT}.jks -storepass ${STOREPASS}
 keytool -exportcert -alias ${ENT} -keystore ${ENT}.jks -storepass ${STOREPASS} -rfc -file ${ENT}.pem
done
####
echo "Creating trust relations"
### IMPORTING TRUSTED CERTIFICATES
### (The server trusts all clients except for client4_J)
### (Every client trusts server_J)
for ENT in client1_J client2_J client3_J; do
 echo "yes"|keytool -import -alias ${ENT} -keystore server_J.jks -file ${ENT}.pem -storepass ${STOREPASS}
 echo "yes"|keytool -import -alias server_J -keystore ${ENT}.jks -file server_J.pem -storepass ${STOREPASS}
done
echo "yes"|keytool -import -alias server_J -keystore client4_J.jks -file server_J.pem -storepass ${STOREPASS}
echo "############################################################################"
keytool -list -keystore server_J.jks -storepass ${STOREPASS}
echo "############################################################################"
echo "WARNING: For testing, client4_J is not added to the list of authorized clients"
echo "############################################################################"
#######
