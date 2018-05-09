# ActiveCM::RITAWriter

## Introduction and Warning

This plugin allows logging to MongoDB in a format appropriate to read by
[RITA (Real Intelligence Threat Analytics)](https://github.com/activecm/rita).
While the basic functionality has been tested and shown to work, it has not
seen much real-life testing and no functionality guarantees are made.

## Installation

**IMPORTANT: BRO IDS MUST BE [INSTALLED FROM SOURCE](https://www.bro.org/sphinx/install/install.html), AND THE SOURCE MUST BE AVAILABLE ON THE SYSTEM**

First, install RITA, Bro IDS, and MongoDB.

Next, install the following dependencies:

Debian packages: `pkg-config libssl-dev libsasl2-dev libsnappy-dev perl make cmake clang gcc g++ git`

Alpine linux packages: `pkgconf openssl-dev cyrus-sasl-dev snappy-dev perl make cmake clang gcc g++ git`


Then, install the MongoDB C++ driver, mongocxx. Official instructions are
provided [here](http://mongodb.github.io/mongo-cxx-driver/mongocxx-v3/installation/).
However, the following script should work on most systems.
```
cd /tmp
wget -q https://github.com/mongodb/mongo-c-driver/releases/download/1.9.2/mongo-c-driver-1.9.2.tar.gz
tar xzf mongo-c-driver-1.9.2.tar.gz
cd mongo-c-driver-1.9.2
./configure --disable-automatic-init-and-cleanup --enable-static
make
sudo make install
cd /tmp
wget -q https://github.com/mongodb/mongo-cxx-driver/archive/r3.1.3.tar.gz
tar xzf r3.1.3.tar.gz
cd mongo-cxx-driver-r3.1.3/build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..
sudo make EP_mnmlstc_core
make
sudo make install
```


After installing mongocxx, install the Bro-RITA plugin
either using bro-pkg, or manually via the command-line.

To install the plugin using [bro-pkg](http://bro-package-manager.readthedocs.io/en/stable/), use:

```
bro-pkg autoconfig
sudo bro-pkg refresh --aggregate
sudo bro-pkg install bro/activecm/bro-rita.git
```

To install manually from the cloned repository, use:

```
sudo ./configure && make && make install
```


## Logging Data into MongoDB Databases for RITA

The easiest way to enable RITA logging is to load the `rita.bro` script
included with the plugin. This script may be loaded via `broctl` by adding
`@load rita.bro` to the `local.bro` policy script. Alternatively,
`rita.bro` may be specified on the command line when running `bro`.

Examples:

```
# Run Bro-RITA such that data is sent to MyDB
bro -r test.pcap rita.bro 'RITAWriter::DB = "MyDB"'

# Run Bro-RITA such that data is sent to MyDB-YYYY-mm-dd-HH-MM on localhost
bro -r test.pcap rita.bro 'RITAWriter::DB = "MyDB"' 'RITAWriter::ROTATE = "true"'

# Run Bro-RITA with a TLS enabled MongoDB server. Assumes a CA signed server certificate
bro -r test.pcap rita.bro 'RITAWriter::URI = "mongodb://other-host:27017?ssl=true"'

# Run Bro-RITA with a TLS enabled MongoDB server and disable server authentication.
bro -r test.pcap rita.bro 'RITAWriter::URI = "mongodb://other-host:27017?ssl=true"' 'RITAWriter::VERIFY_CERT = "false"'

# Run Bro-RITA with a TLS enabled MongoDB server and a custom certificate authority
bro -r test.pcap rita.bro 'RITAWriter::URI = "mongodb://other-host:27017?ssl=true"' 'RITAWriter::CA_FILE = "/path/to/ca/file"'

# Run Bro-RITA with SCRAM-SHA-1 authentication
bro -r test.pcap rita.bro 'RITAWriter::URI = "mongodb://username:password@other-host:27017/auth-db"'

# Run Bro-RITA with X.509 authentication
bro -r test.pcap rita.bro 'RITAWriter::URI = "mongodb://other-host:27017/admin?ssl=true&authMechanism=MONGODB-X509"' 'RITAWriter::CLIENT_CERT = "/path/to/client/cert"'
```

## Configuration options: RITAWriter

The RITAWriter supports the following configuration options via Bro redefines.

- **RITAWriter::URI**: The [MongoDB URI](https://docs.mongodb.com/manual/reference/connection-string/) contains information on how to contact a MongoDB Server.
  - Default value: **mongodb://localhost:27017**

- **RITAWriter::DB**: The name of the database logs will be imported into. If
ROTATE is specified, DB will be used as the base for the names of the databases.
  - Default value: **BRO-IMPORT**

- **RITAWriter::ROTATE**:  ROTATE turns on database rotation similar to that of Bro's ASCII
log writer. Use this option when running RITA each night.
  - Default value: **false**

- **RITAWriter::VERIFY_CERT**: If TLS is enabled (via the URI) server
authentication may be turned off. WARNING: this may lead to MITM attacks.
  - Default value: **true**

- **RITAWriter::CA_FILE**: If TLS is enabled (via the URI) use the given
certificate authority file to validate the server certificate. If a self signed
certificate is being used on the MongoDB server , specify the path to a copy
of that certificate.
  - Default value: **""**

- **RITAWriter::CLIENT_CERT**:  If X.509 authentication is enabled (via the URI), specify the client certificate file.
  - Default value: **""**

## Type mapping

The writer automatically maps the Bro types to the following MongoDB data
types:

| Bro Type                          	| MongoDB Type                            	|
|-----------------------------------	|-----------------------------------------	|
| bool                              	| bool                                    	|
| integer (signed 64 bit)           	| signed 64 bit integer                   	|
| count (unsigned 64 bit integer)   	| signed* 64 bit integer                  	|
| counter (unsigned 64 bit integer) 	| signed* 64 bit integer                  	|
| double                            	| double                                  	|
| time                              	| double / signed 64 bit integer for "ts" 	|
| interval                          	| double                                  	|
| port {enum}                       	| signed 64 bit integer                   	|
| addr                              	| string                                  	|
| subnet                            	| string                                  	|
| enum                              	| string                                  	|
| string                            	| string                                  	|
| func                              	| string                                  	|
| file                              	| string                                  	|
| set[type]                         	| array                                   	|
| vector[type]                      	| array                                   	|

Empty but expected field have various null values place into them.
