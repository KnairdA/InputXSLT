#include "xobject_value.h"

#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XalanDOM/XalanDocumentFragment.hpp>

#include <boost/algorithm/string.hpp>
#include "boost/filesystem.hpp"

#include <string>

#include "support/xalan_string.h"
#include "support/xerces_string_guard.h"

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
	const std::string rawPath(
		this->get<std::string>(ptr)
	);

	if ( auto resolvedPath = this->include_resolver_->resolve(rawPath) ) {
		return *resolvedPath;
	} else {
		return this->filesystem_context_.resolve(rawPath);
	}
}

template <>
xalan::XObjectPtr XObjectValue::get<xalan::XObjectPtr>(
	const xalan::XObjectPtr& ptr) const {
	return ptr;
}

template <>
xalan::XalanNode* XObjectValue::get<xalan::XalanNode*>(
	const xalan::XObjectPtr& ptr) const {
	switch ( ptr->getType() ) {
		case xalan::XObject::eObjectType::eTypeNodeSet: {
			return ptr->nodeset().item(0);
		}
		case xalan::XObject::eObjectType::eTypeResultTreeFrag: {
			return ptr->rtree().getFirstChild();
		}
		default: {
			return nullptr;
		}
	}
}

template <>
xalan::XSLTInputSource XObjectValue::get<xalan::XSLTInputSource>(
	const xalan::XObjectPtr& ptr) const {
	xalan::XalanNode* const node(
		this->get<xalan::XalanNode*>(ptr)
	);

	if ( node == nullptr ) {
		return xalan::XSLTInputSource(
			this->get<boost::filesystem::path>(ptr).string().data()
		);
	} else {
		xalan::XSLTInputSource source(node);

		source.setSystemId(
			*XercesStringGuard<XMLCh>(
				this->filesystem_context_.getBase().string()
			)
		);

		return source;
	}
}

}
