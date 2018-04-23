# @TEST-SERIALIZE: mongodb
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('http').drop()" > /dev/null
# @TEST-EXEC: bro %INPUT || true
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('http').find({}, {'_id': 0})" > write-http.out
# @TEST-EXEC: mongo mongodb://localhost:27017/BRO-BTEST --quiet --eval "db.getCollection('http').drop()" > /dev/null
# @TEST-EXEC: btest-diff write-http.out

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

	Log::add_filter(HTTP::LOG, mongoFilter);

	local empty_set: set[string];
	local empty_tag: set[HTTP::Tags];
	local empty_vector: vector of string;

	local id_record: conn_id = [ 
		$orig_h= 0.0.0.0,
		$orig_p= 0/unknown,
		$resp_h= 0.0.0.0,
		$resp_p= 0/unknown];

	Log::write(HTTP::LOG, [
		$ts=double_to_time(0),
		$uid="",
		$id= id_record,
		$trans_depth= 0,
		$method= "",
		$host= "",
		$uri = "",
		$referrer = "",
		$version = "",
		$user_agent = "",
		$request_body_len= 0,
		$response_body_len = 0 ,
		$status_code = 0,
		$status_msg = "",
		$resp_mime_depth = 0,
		$info_code = 0,
		$info_msg = "",
		$tags= empty_tag,
		$username = "",
		$password = "",
		$proxied = empty_set,
		$orig_fuids = empty_vector,
		$orig_filenames = empty_vector,
		$orig_mime_types = empty_vector,
		$resp_fuids = empty_vector,
		$resp_filenames = empty_vector,
		$resp_mime_types = empty_vector
		]);

}
