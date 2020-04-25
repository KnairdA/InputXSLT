#include "document_cache.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>

#include <stdexcept>

#include "support/xerces_string_guard.h"

namespace InputXSLT {

auto DomDocumentCache::createDocument() -> document_ptr {
	return document_ptr(
		xercesc::DOMImplementation::getImplementation()->createDocument(),
		[](xercesc::DOMDocument* ptr) {
			ptr->release();
		}
	);
}

auto DomDocumentCache::createDocument(const std::string& name) -> document_ptr {
	return document_ptr(
		xercesc::DOMImplementation::getImplementation()->createDocument(
			nullptr,
			*XercesStringGuard<XMLCh>(name),
			nullptr
		),
		[](xercesc::DOMDocument* ptr) {
			ptr->release();
		}
	);
}

DomDocumentCache::DomDocumentCache():
	write_mutex_(),
	cache_() { }

xalan::XalanDocument* DomDocumentCache::create(document_ptr&& document) {
	std::lock_guard<std::mutex> guard(this->write_mutex_);

	this->cache_.emplace(
		std::make_unique<item>(
			std::move(document)
		)
	);

	return this->cache_.top()->getXalanDocument();
}

}
