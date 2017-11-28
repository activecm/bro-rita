event bro_init()
{
    local filter: Log::Filter =
    [
        $name="MongoDB",
        $path="conn",
        $writer=Log::WRITER_MONGODB
    ];
    
    Log::add_filter(Conn::LOG, filter);

    print Log::set_buf(Conn::LOG, F);
}
