#include "support/dom/document_cache_item.h"

#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/util/XMLString.hpp>

#include "support/xerces_string_guard.h"

namespace InputXSLT {

DomDocumentCache::item::item(const std::string& rootNode):
	parser_(),
	dom_support_(parser_),
	document_(
		xercesc::DOMImplementation::getImplementation()->createDocument(
			nullptr,
			*XercesStringGuard(rootNode),
			nullptr
		)
	),
	parsed_source_() { }

DomDocumentCache::item::~item() {
	this->document_->release();
}

bool DomDocumentCache::item::isFinalized() const {
	return static_cast<bool>(this->parsed_source_);
}

xercesc::DOMDocument* DomDocumentCache::item::getXercesDocument() const {
	return this->document_;
}

xalan::XalanDocument* DomDocumentCache::item::getXalanDocument() {
	if ( this->parsed_source_ ) {
		return this->parsed_source_->getDocument();
	} else {
		this->parsed_source_.reset(
			new xalan::XercesDOMWrapperParsedSource(
					document_,
					parser_,
					dom_support_
			)
		);

		return this->parsed_source_->getDocument();
	}
}

}
