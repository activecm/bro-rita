# @TEST-SERIALIZE: mongodb
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('http').drop()" > /dev/null
# @TEST-EXEC: bro %INPUT || true
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('http').find({}, {'_id': 0})" > write-http.out
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('http').drop()" > /dev/null
# @TEST-EXEC: btest-diff write-http.out

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

	Log::add_filter(HTTP::LOG, mongoFilter);

	local tag_set: set[HTTP::Tags] = { "BUY", "USE", "BREAK", "FIX"};
	local proxy_set: set[string] = { "TRASH" , "CHANGE", "MAIL", "UPGRADE"};

        local id_record: conn_id = [ 
		$orig_h= 86.75.30.9,
		$orig_p= 255/icmp,
		$resp_h= 192.168.0.255,
		$resp_p= 501/udp];
        local entity_rec: HTTP::Entity = [ $filename = "TEST HEADER" ];

	Log::write(HTTP::LOG, [
		$ts=double_to_time(121345.0),
		$uid="TESTSTRING",
		$id= id_record,
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
		$tags= tag_set,
		$username = "",
		$password = "",
		$proxied = proxy_set,
		$orig_fuids = vector("CHARGE", "POINT", "ZOOM", "PRESS"),
		$orig_filenames = vector("SNAP", "WORK", "QUICK", "ERASE"),
		$orig_mime_types = vector("WRITE", "CUT", "PASTE", "SAVE"),
		$resp_fuids =  vector("LOAD", "CHECK", "QUICK", "REWRITE"),
		$resp_filenames = vector( "PLUG", "PLAY", "BURN", "RIP" ),
		$resp_mime_types = vector( "DRAG", "DROP", "ZIP", "UNZIP" ),
		$current_entity= entity_rec,
		$orig_mime_depth= 1,
		$resp_mime_depth= 3
		]);

}
