/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/ThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include <iostream>
#include <stdexcept>
#include <sstream>

#include "Classification.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace dataflow;
using namespace shared;

class ClassificationHandler : public ClassificationIf{
public:
    ClassificationHandler()=default;

    void ping() override {cout<<"ping"<<endl;}
    void zip() override { cout << "zip()" << endl; }

    void getStruct(SharedStruct& ret, const int32_t logid) override {
        cout << "getStruct(" << logid << ")" << endl;
        ret = log[logid];
    }

    void Run( ::shared::classes& _return, const  ::shared::Image& im) override{
        cout<<"Run(Image)"<<std::endl;
        cout<<"Image info: c x w x h->";
        printf("%d x %d x %d\n", im.channel,im.width, im.height);
        for(auto i: im.image)
            std::cout<<"server->"<<int(i)<<std::endl;
        
        SharedStruct ss;
        ss.key = 1;
        ss.value = "error flag";
        log[0] = ss;
        std::vector<int16_t> a(5,1);
        _return = a;

        // InvalidOperation io;
        // io.whatOp = 1;
        // io.why = "Test";
        // throw io;

    }
protected:
    map<int32_t, SharedStruct> log;
};


class ClassificationCloneFactory : virtual public ClassificationIfFactory{
public:
    ~ClassificationCloneFactory() override = default;
    ClassificationIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) override
    {
    std::shared_ptr<TSocket> sock = std::dynamic_pointer_cast<TSocket>(connInfo.transport);
    cout << "Incoming connection\n";
    cout << "\tSocketInfo: "  << sock->getSocketInfo() << "\n";
    cout << "\tPeerHost: "    << sock->getPeerHost() << "\n";
    cout << "\tPeerAddress: " << sock->getPeerAddress() << "\n";
    cout << "\tPeerPort: "    << sock->getPeerPort() << "\n";
    return new ClassificationHandler;
    }
    void releaseHandler( ::shared::SharedServiceIf* handler) override {
        delete handler;
  }

};

int main(){
    TThreadedServer server(
    std::make_shared<ClassificationProcessorFactory>(std::make_shared<ClassificationCloneFactory>()),
    std::make_shared<TServerSocket>(9090), //port
    std::make_shared<TBufferedTransportFactory>(),
    std::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
}
