#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "Classification.h"

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
    ClassificationClient client(protocol);

    Image im;
    im.height = 10;
    im.width = 11;
    im.channel = 3;
    im.image = std::vector<int8_t>(10,4);

    try{
        transport->open();
        client.ping();
        cout<<"ping()"<<endl;

        classes result;
        client.Run(result, im);
        cout<<"Run(im) result"<<std::endl;
        for(auto i: result)
            std::cout<<"No: "<<i<<std::endl;

        SharedStruct ss;
        client.getStruct(ss, 0);
        cout << "Received log: " << ss << endl;
    }catch(TException &tx){
        std::cout<<"ERROR: "<<tx.what()<<endl;
    }
}