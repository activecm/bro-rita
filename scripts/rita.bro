module LogMongo;

export {
    const URI           = "mongodb://localhost:27017" &redef;
    const DB            = "BRO-IMPORT" &redef;
    const ROTATE        = "false" &redef;
    const SPLIT_BY_DATE = "false" &redef;
    const TLS           = "false" &redef;
    const CA_FILE       = ""      &redef;
    const VERIFY_CERT   = "true"  &redef;
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
                       ["splitByDate"] = SPLIT_BY_DATE,
                       ["TLS"] = TLS,
                       ["CAFile"] = CA_FILE,
                       ["verifyCert"] = VERIFY_CERT,
                       ["clientCert"] = CLIENT_CERT
                   ),
        $writer=Log::WRITER_MONGODB
    ];

    Log::add_filter(HTTP::LOG, copy(mongoFilter));
    Log::add_filter(Conn::LOG, copy(mongoFilter));
    Log::add_filter(DNS::LOG, copy(mongoFilter));

}
