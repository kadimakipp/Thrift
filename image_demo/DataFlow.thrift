
include "Dtype.thrift"

namespace cpp dataflow
namespace py dataflow



/**
 * Structs can also be exceptions, if they are nasty.
 */
exception InvalidOperation {
  1: i32 whatOp,
  2: string why
}
/**
 * Ahh, now onto the cool part, defining a service. Services just need a name
 * and can optionally inherit from another service using the extends keyword.
 */
service classify extends Dtype.SharedService {

    Dtype.Classification Classify(1:Dtype.Image im),
    /**
   * A method definition looks like C code. It has a return type, arguments,
   * and optionally a list of exceptions that it may throw. Note that argument
   * lists and exception lists are specified using the exact same syntax as
   * field lists in struct or exception definitions.
   */

    i32 test_throws(1:i32 number) throws (1:InvalidOperation poch)

   /**
    * This method has a oneway modifier. That means the client only makes
    * a request and does not listen for any response at all. Oneway methods
    * must be void.
    */
   //oneway void zip()
}

