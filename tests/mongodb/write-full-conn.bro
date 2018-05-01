# @TEST-SERIALIZE: mongodb
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('conn').drop()" > /dev/null
# @TEST-EXEC: bro %INPUT || true
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('conn').find({}, {'_id': 0})" > write-conn.out || true
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('conn').drop()" > /dev/null || true
# @TEST-EXEC: mongo mongodb://localhost:27017/MetaDatabase --quiet --eval "db.getCollection('databases').deleteMany({'name': 'BRO-BTEST'})" > /dev/null || true
# @TEST-EXEC: btest-diff write-conn.out

# Test Record with all possible types.


module RITAWriter;

export {
    const URI           = "mongodb://localhost:27017" &redef;

    const DB            = "BRO-BTEST" &redef;

    const ROTATE        = "false" &redef;

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
                       ["CAFile"] = CA_FILE,
                       ["verifyCert"] = VERIFY_CERT,
                       ["clientCert"] = CLIENT_CERT
                       ),
                $writer=Log::WRITER_RITA
    ];

	Log::add_filter(Conn::LOG, mongoFilter);

        local parent_set: set[string] = { "mother", "father" };

        local id_record: conn_id = [ 
		$orig_h= 86.75.30.9,
		$orig_p= 255/icmp,
		$resp_h= [1234:5678:90AB:CDEF:DEAD:BEEF:C001:0FFF],
		$resp_p= 501/udp];

	Log::write(Conn::LOG, [
		$ts=double_to_time(9000.1),
		$uid="TESTSTRING",
		$id= id_record,
		$proto= tcp,
		$service= "TestService",
		$duration= 9819day,
		$orig_bytes= 44,
		$resp_bytes= 44,
		$conn_state= "AD",
		$local_orig= F,
		$local_resp= T,
		$missed_bytes= 2,
		$history= "h",
		$orig_pkts= 2,
		$orig_ip_bytes= 10,
		$resp_pkts= 1,
		$resp_ip_bytes= 100,
		$tunnel_parents = parent_set
		]);

}
