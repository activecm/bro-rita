event bro_init()
{
    local filter: Log::Filter =
    [
        $name="MongoDB",
        $path="http",
        $writer=Log::WRITER_MONGODB
    ];

    Log::add_filter(HTTP::LOG, filter);
}
