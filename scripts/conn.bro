event bro_init()
{
    local filter: Log::Filter =
    [
        $name="MongoDB",
        $path="conn",
        $config=table(
                ["uri"] = "mongodb://localhost:27017",
                ["selectedDB"]="mydb",
                ["rotate"]="true"
                #["x509ClientCert"]="",
                #["CAFile"]="",
                #["verifyCert"]="true",
                ["selectedDB"]="mydb"
                ),
        $writer=Log::WRITER_MONGODB
    ];

    Log::add_filter(Conn::LOG, filter);
}
