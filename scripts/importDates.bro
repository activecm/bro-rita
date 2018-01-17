event bro_init()
{
    local mongoFilter: Log::Filter =
    [
        $name="MongoDB",
        $config = table(
                       ["uri"] = "mongodb://localhost:27017",
                       ["selectedDB"]="DEMO-PLUGIN-IMPORT-DATES",
                       ["splitOnDay"]="true"
                   ),
        $writer=Log::WRITER_MONGODB
    ];

    Log::add_filter(HTTP::LOG, copy(mongoFilter));
    Log::add_filter(Conn::LOG, copy(mongoFilter));
    Log::add_filter(DNS::LOG, copy(mongoFilter));

}
