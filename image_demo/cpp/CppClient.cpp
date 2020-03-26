#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "classify.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace dataflow;
using namespace shared;

int main()
{
    std::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
    std::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    classifyClient client(protocol);

    Image im;
    im.height = 10;
    im.width = 11;
    im.channel = 3;
    im.data = std::vector<int8_t>(10,4);

    try{
        transport->open();
        Classification result;
        client.Classify(result, im);
        cout<<"Classify result"<<std::endl;
        for(auto i: result.classes)
            std::cout<<"classes: "<<i<<std::endl;
        for(auto i: result.probs)
            std::cout<<"probs: "<<i<<std::endl;
        std::cout<<"test throws"<<std::endl;
        try{
            
            auto r = client.test_throws(5);
            std::cout<<"right ->"<<r<<std::endl;
        }catch (InvalidOperation& io) {
        cout << "InvalidOperation: " << io.why << endl;
        // or using generated operator<<: cout << io << endl;
        // or by using std::exception native method what(): cout << io.what() << endl;
        }
        try{
            auto r = client.test_throws(12);
            std::cout<<"right ->"<<r<<std::endl;
        }catch (InvalidOperation& io) {
        cout << "InvalidOperation: " << io.why << endl;
        // or using generated operator<<: cout << io << endl;
        // or by using std::exception native method what(): cout << io.what() << endl;
        }

        SharedStruct ss;
        client.getStruct(ss, 1);
        cout << "Received log: " << ss << endl;

        transport->close();
    }catch(TException &tx){
        std::cout<<"ERROR: "<<tx.what()<<endl;
    }
}