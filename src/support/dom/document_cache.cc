#include "document_cache.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>

#include <stdexcept>

#include "support/xerces_string_guard.h"

namespace InputXSLT {

auto DomDocumentCache::createDocument() -> document_ptr {
	return document_ptr(
		xercesc::DOMImplementation::getImplementation()->createDocument(
			nullptr,
			*XercesStringGuard<XMLCh>("content"),
			nullptr
		)
	);
}

DomDocumentCache::DomDocumentCache():
	write_mutex_(),
	cache_() { }

xalan::XalanDocument* DomDocumentCache::create(document_ptr&& document) {
	std::lock_guard<std::mutex> guard(this->write_mutex_);

	this->cache_.emplace(
		new item(
			std::move(document)
		)
	);

	return this->cache_.top()->getXalanDocument();
}

void DomDocumentCache::document_deleter::operator()(
	xercesc::DOMDocument* document) {
	document->release();
}

}
