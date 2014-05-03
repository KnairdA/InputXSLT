#include "document_cache.h"

#include <stdexcept>

namespace InputXSLT {

DomDocumentCache::DomDocumentCache():
	write_mutex_(),
	map_() { }

DomDocumentCache::optional_item DomDocumentCache::get(const std::string& key) {
	auto itemIter = this->map_.find(key);

	if ( itemIter == this->map_.end() ) {
		return optional_item(false, nullptr);
	} else {
		return optional_item(true, (*itemIter).second.get());
	}
}

DomDocumentCache::optional_item DomDocumentCache::create(
	const std::string&    key,
	xercesc::DOMDocument* document
) {
	std::lock_guard<std::mutex> guard(this->write_mutex_);

	auto result = this->map_.emplace(
		std::make_pair(key, std::unique_ptr<item>(new item(document)))
	);

	if ( result.second ) {
		return optional_item(true, (*(result.first)).second.get());
	} else {
		return optional_item(false, nullptr);
	}
}

}
