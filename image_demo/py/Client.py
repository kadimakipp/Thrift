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
import numpy as np
import cv2
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
    im_h = 1200
    im_w = 1920
    im_c = 3
    img = np.zeros((im_h,im_w,im_c),dtype=np.uint8)
    for y in np.arange(0,im_h):
        for x in np.arange(0,im_w):
            img[y,x,:] = (y*im_h+x)%255

    #img = cv2.imread("../build/1584428668870.jpg")

    h,w,c = img.shape
    img = img.reshape(1,-1).tostring()
    im = Image(height=h, width=w, channel=c, data = img)
    print("c x w x h->",im.channel, im.width, im.height)
    #print("im.data", im.data)
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
