#ifndef INPUTXSLT_SRC_SUPPORT_XALAN_STRING_H_
#define INPUTXSLT_SRC_SUPPORT_XALAN_STRING_H_

#include <xalanc/XalanDOM/XalanDOMString.hpp>

#include <string>

#include "common.h"

namespace InputXSLT {

std::string toString(const xalan::XalanDOMString&);
xalan::XalanDOMString toString(const std::string&);

}

#endif  // INPUTXSLT_SRC_SUPPORT_XALAN_STRING_H_
