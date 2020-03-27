import glob
import sys
sys.path.append('../gen-py')

from dataflow import classify
from dataflow.ttypes import InvalidOperation

from shared.ttypes import SharedStruct, Image, Classification

from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer
import numpy as np
import cv2
from ktimer import Stopwatch
class classifyHandler:
    def __init__(self):
        self.log = {}

    def Classify(self, im):
        stopwatch = Stopwatch()
        print("c x w x h->",im.channel, im.width, im.height)
        print("im.data type->", type(im.data))
        img = np.array(list(im.data)).astype(np.uint8).reshape(im.height, im.width, im.channel)
        # cv2.imwrite("recever.jpg", img)
        flag = 0
        for y in np.arange(0,im.height):
            for x in np.arange(0,im.width):
                value = (y*im.height+x)%255
                if (img[y,x,:]==value).all():
                    flag=1
        if flag:
            print("right")
        else:
            print("error")
                    
        log = SharedStruct()
        log.key = 0
        log.value = 'err'
        self.log[1] = log

        stopwatch.stop()
        print("server time:", str(stopwatch))
        return Classification(classes=[1,2,3,4,5], probs=[0.1,0.2,0.3,0.4,0.5])

    def getStruct(self, key):
        print('getStruct(%d)' % (key))
        return self.log[key]

    def test_throws(self, number):
        print("test throws")
        if number>10:
            x = InvalidOperation()
            x.whatOp = number
            x.why = "Test invalid operation"
            raise x
        return number

if __name__ == '__main__':
    handler = classifyHandler()
    processor = classify.Processor(handler)
    transport = TSocket.TServerSocket(host='127.0.0.1', port=9090)
    tfactory = TTransport.TBufferedTransportFactory()
    pfactory = TBinaryProtocol.TBinaryProtocolFactory()

    server = TServer.TSimpleServer(processor, transport, tfactory, pfactory)

    # You could do one of these for a multithreaded server
    # server = TServer.TThreadedServer(
    #     processor, transport, tfactory, pfactory)
    # server = TServer.TThreadPoolServer(
    #     processor, transport, tfactory, pfactory)

    print('Starting the server...')
    server.serve()
    print('done.')