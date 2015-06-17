#ifndef CmdCenterDaemon_H
#define CmdCenterDaemon_H

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using namespace cmdcenterstubs;
using namespace qastubs;

class BadImgFileException {};

class AssignmentFailedException{
public:
	AssignmentFailedException(std::string msg) {
		err = msg;
	}
	string err;
};

class ServiceData
{
public:
	ServiceData(std::string hostname, int port) {
	  	socket = boost::shared_ptr<TTransport>(new TSocket(hostname, port));
	  	transport = boost::shared_ptr<TTransport>(new TBufferedTransport(socket));
	  	protocol = boost::shared_ptr<TProtocol>(new TBinaryProtocol(transport));
	}
	ServiceData(ServiceData *sd) {
	  	socket = sd->socket;
	  	transport = sd->transport;
	  	protocol = sd->protocol;
	}
	boost::shared_ptr<TTransport> socket;
	boost::shared_ptr<TTransport> transport;
	boost::shared_ptr<TProtocol> protocol;
};

// NOTE: why am I not using get() and set()? ~Ben
// set() allows you to encapsulate assignment validity checks
// within the class. However, it's difficult to determine whether
// audio/img data is well-formed from within the command center.
class AsrServiceData : public ServiceData
{
public:
	AsrServiceData(ServiceData *sd)
	: ServiceData(sd), client(protocol), audio("") {}
	KaldiServiceClient client;
	std::string audio;
};

class QaServiceData : public ServiceData
{
public:
	QaServiceData(ServiceData *sd)
	: ServiceData(sd), client(protocol) {}
	QAServiceClient client;
};

class ImmServiceData : public ServiceData
{
public:
	ImmServiceData(ServiceData *sd)
	: ServiceData(sd), client(protocol), img("") {}
	ImageMatchingServiceClient client;
	std::string img;
};

class ResponseData
{
public:
	ResponseData(std::string _response)
	: response(_response) {}
	std::string getResponse() { return response; }
private:
	// could be audio transcript, answer, or matched img name
	std::string response;
};

#endif