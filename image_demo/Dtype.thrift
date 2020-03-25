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
    3: i8 channel
    4: list<i8> image
}
typedef list<i16> classes


typedef list<double> bbox

typedef list<bbox> bboxes

struct Detection {
    1: classes class_num
    2: bboxes  bb
}

struct SharedStruct {
  1: i32 key
  2: string value
}

service SharedService {
  SharedStruct getStruct(1: i32 key)
}