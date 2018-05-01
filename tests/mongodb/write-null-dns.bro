# @TEST-SERIALIZE: mongodb
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('dns').drop()" > /dev/null
# @TEST-EXEC: bro %INPUT || true
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('dns').find({}, {'_id': 0})" > write-dns.out
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('dns').drop()" > /dev/null
# @TEST-EXEC: mongo mongodb://localhost:27017/MetaDatabase --quiet --eval "db.getCollection('databases').deleteMany({'name': 'BRO-BTEST'})" > /dev/null || true
# @TEST-EXEC: btest-diff write-dns.out

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

	Log::add_filter(DNS::LOG, mongoFilter);

	local empty_string_vector: vector of string;
	local empty_interval_vector: vector of intervals

        local id_record: conn_id = [ 
		$orig_h= 0.0.0.0,
		$orig_p= 0/unknown,
		$resp_h= 0.0.0.0,
		$resp_p= 0/unknown];

	Log::write(DNS::LOG, [
		$ts=double_to_time(0),
		$uid="",
		$id= id_record,
		$proto= unknown_transport,
		$trans_id= 0,
		$rtt= 0usec,
		$query= "",
		$qclass= 0,
		$qclass_name= "",
		$rcode= 0,
		$rcode_name= "",
		$AA= F,
		$TC= F,
		$RD= F,
		$RA= F,
		$Z= 0,
		$answers= empty_string_vector,
		$TTLs= empty_interval_vector,
		$rejected= F,
		$total_answers= 0,
		$total_replies= 0,
		$saw_query= F,
		$saw_reply= F
		]);


}
