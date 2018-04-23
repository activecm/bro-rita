# @TEST-SERIALIZE: mongodb
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('dns').drop()" > /dev/null
# @TEST-EXEC: bro %INPUT || true
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('dns').find({}, {'_id': 0})" > write-dns.out
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('dns').drop()" > /dev/null
# @TEST-EXEC: btest-diff write-dns.out

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

	Log::add_filter(DNS::LOG, mongoFilter);

        local id_record: conn_id = [ 
		$orig_h= 86.75.30.9,
		$orig_p= 255/icmp,
		$resp_h= 192.168.0.255,
		$resp_p= 501/udp];

	Log::write(DNS::LOG, [
		$ts=double_to_time(121345.0),
		$uid="TESTSTRING",
		$id= id_record,
		$proto= tcp,
		$trans_id= 12,
		$rtt= 525600min,
		$query= "www.microTEST.com",
		$qclass= 32767,
		$qclass_name= "Half Standard Action",
		$rcode= 1,
		$rcode_name= "For Me R2",
		$AA= T,
		$TC= F,
		$RD= F,
		$RA= F,
		$Z= 0,
		$answers= vector( "> twelve seconds" ),
		$TTLs= vector( 11sec ),
		$rejected= F,
		$total_answers= 1,
		$total_replies= 1,
		$saw_query= T,
		$saw_reply= F
		]);


}
