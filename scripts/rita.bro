module RITAWriter;

export {
    # The MongoDB URI contains information on how to contact a MongoDB Server
    # https://docs.mongodb.com/manual/reference/connection-string/
    const URI           = "mongodb://localhost:27017" &redef;

    # Set the name of the database logs will be imported into.
    # If ROTATE or SPLIT_BY_DATE is specified, DB will be used as
    # the base for the names of the databases.
    const DB            = "BRO-IMPORT" &redef;

    # ROTATE turns on database rotation similar to that of Bro's ASCII log writer.
    # Use this option when running RITA each night.
    const ROTATE        = "false" &redef;


    # Security

    # If TLS is enabled (via the URI) and a self signed certificate is being used
    # on the MongoDB server, specify the path to a copy of that certificate.
    const CA_FILE       = ""      &redef;

    # As a quick solution, certificate validation may be turned off instead of
    # specifying the certificate file. WARNING: this may lead to MITM attacks.
    const VERIFY_CERT   = "true"  &redef;

    # If using x509 authentication, specify the client certificate.
    const CLIENT_CERT   = ""      &redef;
}

event bro_init()
{
    local mongoFilter: Log::Filter =
    [
        $name="MongoDB",
        $config = table(
                       ["uri"] = URI,
                       ["selectedDB"] = DB,
                       ["rotate"] = ROTATE,
                       ["CAFile"] = CA_FILE,
                       ["verifyCert"] = VERIFY_CERT,
                       ["clientCert"] = CLIENT_CERT
                   ),
        $writer=Log::WRITER_RITA
    ];

    Log::add_filter(HTTP::LOG, copy(mongoFilter));
    Log::add_filter(Conn::LOG, copy(mongoFilter));
    Log::add_filter(DNS::LOG, copy(mongoFilter));

}
