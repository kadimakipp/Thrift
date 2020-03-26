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

#include "classify.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace dataflow;
using namespace shared;

class classifyHandler : public classifyIf{
public:
    classifyHandler()=default;

    int32_t test_throws(const int32_t number) override {
    // Your implementation goes here
        printf("test_throws\n");
        std::cout<<"param number->"<<number<<std::endl;
        if(number>10)
        {
            InvalidOperation io;
            io.whatOp = number;
            io.why = "Test invalid operation";
            throw io;
        }

        return number;
    }

    void getStruct(SharedStruct& ret, const int32_t logid) override {
        cout << "getStruct(" << logid << ")" << endl;
        ret = log[logid];
    }

    void Classify( ::shared::Classification& _return, const  ::shared::Image& im) override{
        cout<<"Run(Image)"<<std::endl;
        cout<<"Image info: c x w x h->";
        printf("%d x %d x %d\n", im.channel,im.width, im.height);
        for(auto i: im.data)
            std::cout<<"server->"<<int(i)<<std::endl;
        
        Classification classes;
        classes.classes = std::vector<int16_t>(5,1);
        classes.probs = std::vector<double>(5, 0.01);
        _return = classes;

        SharedStruct ss;
        ss.key = 1;
        ss.value = "error flag";
        log[1] = ss;
    }
protected:
    map<int32_t, SharedStruct> log;
};


class classifyCloneFactory : virtual public classifyIfFactory{
public:
    ~classifyCloneFactory() override = default;
    classifyIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) override
    {
    std::shared_ptr<TSocket> sock = std::dynamic_pointer_cast<TSocket>(connInfo.transport);
    cout << "Incoming connection\n";
    cout << "\tSocketInfo: "  << sock->getSocketInfo() << "\n";
    cout << "\tPeerHost: "    << sock->getPeerHost() << "\n";
    cout << "\tPeerAddress: " << sock->getPeerAddress() << "\n";
    cout << "\tPeerPort: "    << sock->getPeerPort() << "\n";
    return new classifyHandler;
    }
    void releaseHandler( ::shared::SharedServiceIf* handler) override {
        delete handler;
  }

};

int main(){
    TThreadedServer server(
    std::make_shared<classifyProcessorFactory>(std::make_shared<classifyCloneFactory>()),
    std::make_shared<TServerSocket>(9090), //port
    std::make_shared<TBufferedTransportFactory>(),
    std::make_shared<TBinaryProtocolFactory>());

    cout << "Starting the server..." << endl;
    server.serve();
    cout << "Done." << endl;
}
