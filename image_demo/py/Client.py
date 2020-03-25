import sys
import glob
sys.path.append('../gen-py')

from dataflow import classify
from dataflow.ttypes import InvalidOperation
from shared.ttypes import Image

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

def main():
    # Make socket
    transport = TSocket.TSocket('localhost', 9090)

    # Buffering is critical. Raw sockets are very slow
    transport = TTransport.TBufferedTransport(transport)

    # Wrap in a protocol
    protocol = TBinaryProtocol.TBinaryProtocol(transport)

    # Create a client to use the protocol encoder
    client = classify.Client(protocol)

    # Connect!
    transport.open()

    im = Image(height=10, width=20, channel=3, data = [11,22,33,44,55])
    print("c x w x h->",im.channel, im.width, im.height)
    print("im.data", im.data)
    print("im.data type->", type(im.data))
    
    result = client.Classify(im)
    for r in result.classes:
        print("get->", r)
    for p in result.probs:
        print("prob->", p)

    try:
        quotient = client.test_throws(1)
        print('test_throws the answer is %d' % quotient)
    except InvalidOperation as e:
        print('InvalidOperation: %r' % e)

    try:
        quotient = client.test_throws(12)
        print('test_throws the answer is %d' % quotient)
    except InvalidOperation as e:
        print('InvalidOperation: %r' % e)

    log = client.getStruct(1)
    print('Check log: %s' % log.value)

    # Close!
    transport.close()

if __name__ == '__main__':
    try:
        main()
    except Thrift.TException as tx:
        print('%s' % tx.message)
