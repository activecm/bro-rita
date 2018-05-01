# @TEST-SERIALIZE: mongodb
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('conn').drop()" > /dev/null
# @TEST-EXEC: bro %INPUT || true
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('conn').find({}, {'_id': 0})" > write-conn.out
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('conn').drop()" > /dev/null
# @TEST-EXEC: mongo mongodb://localhost:27017/MetaDatabase --quiet --eval "db.getCollection('databases').deleteMany({'name': 'BRO-BTEST'})" > /dev/null || true
# @TEST-EXEC: btest-diff write-conn.out

# Test Record with all possible null types.


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

	local empty_set: set[string];
        local empty_endpoint: endpoint;

        local id_record: conn_id = [ 
		$orig_h= 0.0.0.0,
		$orig_p= 0/unknown,
		$resp_h= 0.0.0.0,
		$resp_p= 0/unknown];

	Log::write(Conn::LOG, [
		$ts=double_to_time(0),
		$uid="",
		$id= id_record,
		$proto= unknown_transport,
		$service= "",
		$duration= 0usec,
		$orig_bytes= 0,
		$resp_bytes= 0,
		$conn_state= "",
		$local_orig= F,
		$local_resp= F,
		$missed_bytes= 0,
		$history= "",
		$orig_pkts= 0,
		$orig_ip_bytes= 0,
		$resp_pkts= 0,
		$resp_ip_bytes= 0,
		$tunnel_parents = empty_set
		]);

}
