#ifndef INPUTXSLT_SRC_SUPPORT_TUPLE_XOBJECT_VALUE_H_
#define INPUTXSLT_SRC_SUPPORT_TUPLE_XOBJECT_VALUE_H_

#include <xalanc/XPath/XObject.hpp>

#include "common.h"

namespace InputXSLT {

namespace XObjectValue {
	template <typename Type>
	Type get(const xalan::XObjectPtr&);
}

}

#endif  // INPUTXSLT_SRC_SUPPORT_TUPLE_XOBJECT_VALUE_H_
