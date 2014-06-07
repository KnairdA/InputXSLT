#include "xobject_value.h"

#include <boost/algorithm/string.hpp>

#include <string>

#include "support/xalan_string.h"

namespace InputXSLT {

namespace XObjectValue {

template <>
std::string get<std::string>(const xalan::XObjectPtr& ptr) {
	return boost::trim_copy(toString(ptr->str()));
}

template <>
xalan::XObjectPtr get<xalan::XObjectPtr>(const xalan::XObjectPtr& ptr) {
	return ptr;
}

}

}
