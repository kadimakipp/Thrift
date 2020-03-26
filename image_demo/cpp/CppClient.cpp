#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "classify.h"
#ifdef IMageTest
#include <opencv2/opencv.hpp>
#else
#include <opencv2/core.hpp>
#endif

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
#ifdef IMageTest
    cv::Mat img = cv::imread("1584428668870.jpg");
#else
    cv::Mat img = cv::Mat::zeros(100,120,CV_8UC3);
    for(int y=0;y<img.rows;y++)
        for(int x=0;x<img.cols;x++)
        {
            auto value = (y*img.rows+x)%255;
            img.at<cv::Vec3b>(y,x)[0] = value;
            img.at<cv::Vec3b>(y,x)[1] = value;
            img.at<cv::Vec3b>(y,x)[2] = value;
        }
#endif
    Image im;
    im.height = img.rows;
    im.width = img.cols;
    im.channel = img.channels();
    //flatten mat
    unsigned int imtotal = img.total()*img.channels();
    im.data = std::string(imtotal, 0);
    memcpy((void *)im.data.data(), img.data, imtotal*sizeof(uchar));

    try{
        transport->open();
        Classification result;
        client.Classify(result, im);
        std::cout<<"Classify result"<<std::endl;
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