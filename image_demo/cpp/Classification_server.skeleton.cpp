// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "Classification.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <iostream>
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::dataflow;
using namespace  ::shared;

class ClassificationHandler : virtual public ClassificationIf {
 public:
  ClassificationHandler() {
    // Your initialization goes here
  }

  /**
   * A method definition looks like C code. It has a return type, arguments,
   * and optionally a list of exceptions that it may throw. Note that argument
   * lists and exception lists are specified using the exact same syntax as
   * field lists in struct or exception definitions.
   */
  void ping() {
    // Your implementation goes here
    printf("ping\n");
  }

  void Run( ::shared::classes& _return, const  ::shared::Image& im) {
    // Your implementation goes here
    printf("Run\n");
  }

  /**
   * This method has a oneway modifier. That means the client only makes
   * a request and does not listen for any response at all. Oneway methods
   * must be void.
   */
  void zip() {
    // Your implementation goes here
    printf("zip\n");
  }

  void getStruct(SharedStruct& _return, const int32_t key) {
    // Your implementation goes here
    printf("getStruct\n");
  }
protected:
  std::map<int32_t, SharedStruct> log;
};

int main(int argc, char **argv) {
  int port = 9090;
  ::std::shared_ptr<ClassificationHandler> handler(new ClassificationHandler());
  ::std::shared_ptr<TProcessor> processor(new ClassificationProcessor(handler));
  ::std::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  ::std::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  ::std::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}
