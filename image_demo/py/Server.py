import glob
import sys
sys.path.append('../gen-py')

from dataflow import Classification
from dataflow.ttypes import InvalidOperation

from shared.ttypes import SharedStruct, Image

from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer

class ClassificationHandler:
    def __init__(self):
        self.log = {}

    def ping(self):
        print('ping()')

    def Run(self, im):
        print("c x w x h->",im.channel, im.width, im.height)
        print("im.image", im.image)
        print("im.image type->", type(im.image))
        log = SharedStruct()
        log.key = 0
        log.value = 'err'
        self.log[1] = log

        return [1,2,3,4,5]

    def getStruct(self, key):
        print('getStruct(%d)' % (key))
        return self.log[key]

    def zip(self):
        print('zip()')

if __name__ == '__main__':
    handler = ClassificationHandler()
    processor = Classification.Processor(handler)
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