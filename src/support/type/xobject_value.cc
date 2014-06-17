#include "xobject_value.h"

#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XalanDOM/XalanDocumentFragment.hpp>

#include <boost/algorithm/string.hpp>
#include "boost/filesystem.hpp"

#include <string>

#include "support/xalan_string.h"

namespace InputXSLT {

XObjectValue::XObjectValue(
	const boost::filesystem::path& path,
	const IncludeEntityResolver*   resolver
):
	filesystem_context_(path),
	include_resolver_(resolver) { }

template <>
std::string XObjectValue::get<std::string>(
	const xalan::XObjectPtr& ptr) const {
	return boost::trim_copy(toString(ptr->str()));
}

template <>
boost::filesystem::path XObjectValue::get<boost::filesystem::path>(
	const xalan::XObjectPtr& ptr) const {
	const boost::filesystem::path rawPath(
		toString(ptr->str())
	);

	const boost::filesystem::path filePath(
		this->filesystem_context_.resolve(rawPath)
	);

	if ( !(boost::filesystem::exists(filePath) &&
		   boost::filesystem::is_regular_file(filePath)) ) {
		if ( auto resolvedPath = this->include_resolver_->resolve(rawPath) ) {
			return *resolvedPath;
		} else {
			return filePath;
		}
	} else {
		return filePath;
	}
}

template <>
xalan::XObjectPtr XObjectValue::get<xalan::XObjectPtr>(
	const xalan::XObjectPtr& ptr) const {
	return ptr;
}

template <>
xalan::XSLTInputSource XObjectValue::get<xalan::XSLTInputSource>(
	const xalan::XObjectPtr& ptr) const {
	switch ( ptr->getType() ) {
		case xalan::XObject::eObjectType::eTypeNodeSet: {
			return xalan::XSLTInputSource(
				ptr->nodeset().item(0)
			);
		}
		case xalan::XObject::eObjectType::eTypeResultTreeFrag: {
			return xalan::XSLTInputSource(
				ptr->rtree().getFirstChild()
			);
		}
		default: {
			return xalan::XSLTInputSource(
				this->get<boost::filesystem::path>(ptr).string().data()
			);
		}
	}
}

}
