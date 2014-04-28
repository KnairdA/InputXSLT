#include "document_cache.h"

#include <stdexcept>

namespace InputXSLT {

DomDocumentCache::DomDocumentCache():
	map_() { }

DomDocumentCache::item* DomDocumentCache::get(const std::string& key) {
	auto itemIter = this->map_.find(key);

	if ( itemIter == this->map_.end() ) {
		auto result = this->map_.emplace(
			std::make_pair(key, std::unique_ptr<item>(new item("content")))
		);

		if ( result.second ) {
			return (*(result.first)).second.get();
		} else {
			throw std::out_of_range("failed to instantiate DomDocumentCache");
		}
	} else {
		return (*itemIter).second.get();
	}
}

}
