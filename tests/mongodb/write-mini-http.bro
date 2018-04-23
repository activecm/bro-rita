# @TEST-SERIALIZE: mongodb
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('http').drop()" > /dev/null
# @TEST-EXEC: bro %INPUT || true
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('http').find({}, {'_id': 0})" > write-http.out
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('http').drop()" > /dev/null
# @TEST-EXEC: btest-diff write-http.out

# Test all possible types.


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

	Log::add_filter(HTTP::LOG, mongoFilter);

	local tag_set: set[HTTP::Tags] = { HTTP::EMPTY };

        local id_record: conn_id = [ 
		$orig_h= 86.75.30.9,
		$orig_p= 255/icmp,
		$resp_h= 192.168.0.255,
		$resp_p= 501/udp];

	Log::write(HTTP::LOG, [
		$ts=double_to_time(121345.0),
		$uid="TESTSTRING",
		$id= id_record,
		$trans_depth= 7,
		$tags=  tag_set
		]);

}
