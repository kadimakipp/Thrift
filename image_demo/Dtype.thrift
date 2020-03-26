/**
*
* 图片的基本数据类型和结构体的
* 主要用于深度学习部署时，给python 服务器发送图片幷让cpp 客户端接收结果。
*
*
*
*/

namespace cpp shared
namespace py  shared


struct Image{
    1: i16 height
    2: i16 width
    3: i16 channel
    4: binary data
}
typedef list<i16> categories
typedef list<double> probabilitise

struct Classification {
    1: categories classes
    2: probabilitise probs
}

typedef list<double> bbox
typedef list<bbox> bboxes

struct Detection {
    1: categories classes
    2: probabilitise probs
    3: bboxes  bb
}

struct SharedStruct {
  1: i32 key
  2: string value
}

service SharedService {
  SharedStruct getStruct(1: i32 key)
}