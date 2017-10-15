#ifndef LOGGING_WRITER_MONGO_H
#define LOGGING_WRITER_MONGO_H
namespace logging { namespace writer {

class MongoDB : public WriterBackend {

public:
	MongoDB(WriterFrontend* frontend);
	~MongoDB();

	// prohibit copying and moving
	MongoDB(const MongoDB&) = delete;
	MongoDB& operator=(const MongoDB&) = delete;
	MongoDB(MongoDB&&) = delete;

	static WriterBackend* Instantiate(WriterFrontend* frontend)
		{ return new MongoDB(frontend); }

protected:
	bool DoInit(const WriterInfo& info, int num_fields, const threading::Field* const* fields) override;
	bool DoWrite(int num_fields, const threading::Field* const* fields, threading::Value** vals) override;
	bool DoSetBuf(bool enabled) override;
	bool DoRotate(const char* rotated_path, double open, double close, bool terminating) override;
	bool DoFlush(double network_time) override;
	bool DoFinish(double network_time) override;
	bool DoHeartbeat(double network_time, double current_time) override;

}

}
#endif
