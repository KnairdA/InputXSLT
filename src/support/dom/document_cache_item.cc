#include "support/dom/document_cache_item.h"

namespace InputXSLT {

DomDocumentCache::item::item(document_ptr&& document):
	document_(std::move(document)),
	parser_(),
	dom_support_(parser_),
	parsed_source_(
		document_.get(),
		parser_,
		dom_support_
	) { }

xalan::XalanDocument* DomDocumentCache::item::getXalanDocument() {
	return this->parsed_source_.getDocument();
}

}
