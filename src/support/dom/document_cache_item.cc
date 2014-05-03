#include "support/dom/document_cache_item.h"

namespace InputXSLT {

DomDocumentCache::item::item(xercesc::DOMDocument* document):
	parser_(),
	dom_support_(parser_),
	document_(document),
	parsed_source_(
		document_,
		parser_,
		dom_support_
	) { }

DomDocumentCache::item::~item() {
	this->document_->release();
}

xalan::XalanDocument* DomDocumentCache::item::getXalanDocument() {
	return this->parsed_source_.getDocument();
}

}
