event bro_init()
{

    local mongoConfig: table[string] of string = table(
        ["uri"] = "mongodb://localhost:27017",
        ["selectedDB"]="DEMO-PLUGIN_IMPORT_DATES",
        ["rotate"]="true"
    );

    local connFilter: Log::Filter =
    [
        $name="MongoDB",
        $path="conn",
        $config=mongoConfig,
        $writer=Log::WRITER_MONGODB
    ];

    local httpFilter: Log::Filter =
    [
        $name="MongoDB",
        $path="http",
        $config=mongoConfig,
        $writer=Log::WRITER_MONGODB
    ];

    local dnsFilter: Log::Filter =
    [
        $name="MongoDB",
        $path="dns",
        $config=mongoConfig,
        $writer=Log::WRITER_MONGODB
    ];


    Log::add_filter(Conn::LOG, connFilter);
    Log::add_filter(HTTP::LOG, httpFilter);
    Log::add_filter(DNS::LOG, dnsFilter);

}
