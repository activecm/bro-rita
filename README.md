Logging to MongoDB with ActiveCM::RITAWriter
=================================

Introduction and Warning
========================

This plugin allows logging to as well as reading from MongoDB
databases. While the basic functionality seems to work, it has not
seen much real-life testing and no functionality guarantees are made.

Installation
------------

After installing MongoDB, you can install the Bro MongoDB module
either using bro-pkg, or manually via the command-line.

To install the plugin using bro-pkg, use

```console
# bro-pkg install activeCM/bro-mongodb
```

To install manually from the cloned repository, use::

```console
# ./configure && make && make install
```


Logging Data into MongoDB databases
-------------------------------------

The easiest way to add MongoDB logging is by adding a logging filter to an
already existing logging stream. This first example also sends the conn.log
to MongoDB:

```bro
event bro_init()
{
    local mongoFilter: Log::Filter =
    [
        $name="MongoDB",
        $config = table(
                       ["uri"] = "monbodb://localhost:27017",
                       ["selectedDB"] = "mydb",
                       ["rotate"] = false,
                       ["CAFile"] = "",
                       ["verifyCert"] = "",
                       ["clientCert"] = ""
                   ),
        $writer=Log::WRITER_RITA
    ];
    Log::add_filter(Conn::LOG, copy(mongoFilter));

}
```

This will write to a database named mydb on a locally hosted MongoDB database
running on port 27017. Note that the table will be automatically be created by
the MongoDB plugin, if it does not yet exist. If a table with the specified
name already exists, it is used.

Type mapping
============

The writer automatically maps the Bro types to the following MongoDB data
types:

<table>
<tr>
<th>Bro type</th>
<th>MongoDB type</th>
</tr><tr><td>Bool</td><td>bool</td>
</tr><tr><td>int</td><td>long</td>
</tr><tr><td>count</td><td>long</td>
</tr><tr><td>port</td><td>int</td>
</tr><tr><td>addr</td><td>string</td>
</tr><tr><td>subnet</td><td>string</td>
</tr><tr><td>time</td><td>long</td>
</tr><tr><td>interval</td><td>long</td>
</tr><tr><td>double</td><td>long</td>
</tr><tr><td>enum</td><td>string</td>
</tr><tr><td>string</td><td>string</td>
</tr><tr><td>func</td><td>string</td>
</tr><tr><td>set[type]</td><td>array</td>
</tr><tr><td>vector[type]</td><td>array</td>
</tr>
</table>

Empty but expected field have various null values place into them.


Configuration options: MongoDB Writer
========================================

The MongoDB writer supports the following configuration options that can be
passed in $config:

- *uri*: The MongoDB URI contains information on how to contact a MongoDB Server
https://docs.mongodb.com/manual/reference/connection-string/

- *selectedDB*: Set the name of the database logs will be imported into. If
ROTATE or SPLIT_BY_DATE is specified, DB will be used as the base for the names of the databases.

- *rotate*:  ROTATE turns on database rotation similar to that of Bro's ASCII
log writer. Use this option when running RITA each night.

- *CAFile*: If TLS is enabled (via the URI) and a self signed certificate is
being used on the MongoDB server, specify the path to a copy of that certificate.

- *verifyCert*: As a quick solution, certificate validation may be turned off
instead of specifying the certificate file. WARNING: this may lead to MITM attacks.

- *clientCert*:  If using x509 authentication, specify the client certificate file.
