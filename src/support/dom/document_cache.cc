#include "document_cache.h"

#include <stdexcept>

namespace InputXSLT {

DomDocumentCache::DomDocumentCache():
	write_mutex_(),
	cache_() { }

xalan::XalanDocument* DomDocumentCache::create(xercesc::DOMDocument* document) {
	std::lock_guard<std::mutex> guard(this->write_mutex_);

	this->cache_.emplace(new item(document));

	return this->cache_.top()->getXalanDocument();
}

}
