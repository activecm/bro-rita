event bro_init()
{
    local filter: Log::Filter =
    [
        $name="MongoDB",
        $path="dns",
        $writer=Log::WRITER_MONGODB
    ];

    Log::add_filter(DNS::LOG, filter);
}
