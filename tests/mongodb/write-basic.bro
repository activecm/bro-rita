# @TEST-SERIALIZE: mongodb
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('http').drop()" > /dev/null
# @TEST-EXEC: bro %INPUT || true
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('http').find({}, {'_id': 0})" > write-basic.out
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('http').drop()" > /dev/null
# @TEST-EXEC: btest-diff write-basic.out

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

	local empty_set: set[string];
	local empty_tag: set[HTTP::Tags];
	local empty_vector: vector of string;

	Log::write(HTTP::LOG, [
		$ts=double_to_time(121345.0),
		$uid="TESTSTRING",
		$trans_depth= 7,
		$method= "POST",
		$host= "www.microTEST.com",
		$uri = "/isapi/redir.dll?prd=ie&pver=6&ar=msnhome",
		$referrer = "",
		$version = "",
		$user_agent = "TESTzilla/4.0",
		$request_body_len= 0,
		$response_body_len = 123 ,
		$status_code = 302,
		$status_msg = "Moved Temporarily",
		$resp_mime_depth = 2,
		$info_code = 100,
		$info_msg = "",
		$tags= empty_tag,
		$username = "",
		$password = "",
		$proxied = empty_set,
		$orig_fuids = empty_vector,
		$orig_filenames = empty_vector,
		$orig_mime_types = empty_vector,
		$resp_fuids = vector( "FE8SI5OPHP82RdKY3" ),
		$resp_filenames = empty_vector,
		$resp_mime_types = vector("text/html" )
		]);

#	Log::write(Conn::LOG, [
#		$ts=network_time(),
#		$uid="TESTSTRING",
#		$trans_depth= 7,
#		$id_origin_h= "86.75.30.9",
#		$id_origin_p= 404,
#		$id_resp_h= "192.168.0.255",
#		$id_resp_p= 501,
#		$proto= "udp",
#		$service= "dns",
#		$duration= .01,
#		$orig_bytes= 44,
#		$resp_bytes= 44,
#		$conn_state= "AD",
#		$local_orig= F,
#		$local_resp= T,
#		$missed_bytes= 2,
#		$history= "Dd",
#		$orig_pkts= 2,
#		$orig_ip_bytes= 10,
#		$resp_pkts= 1,
#		$resp_ip_bytes= 1,
#		$tunnel_parents = empty_vector
#		]);

}
