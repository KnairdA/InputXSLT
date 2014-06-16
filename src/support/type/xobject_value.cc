#include "xobject_value.h"

#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XalanDOM/XalanDocumentFragment.hpp>

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

template <>
xalan::XSLTInputSource get<xalan::XSLTInputSource>(
	const xalan::XObjectPtr& ptr) {
	switch ( ptr->getType() ) {
		case xalan::XObject::eObjectType::eTypeNodeSet: {
			return xalan::XSLTInputSource(ptr->nodeset().item(0));
		}
		case xalan::XObject::eObjectType::eTypeResultTreeFrag: {
			return xalan::XSLTInputSource(ptr->rtree().getFirstChild());
		}
		default: {
			return xalan::XSLTInputSource(ptr->str());
		}
	}
}

}

}
