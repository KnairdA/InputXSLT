#include "document_cache.h"

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
			return nullptr;
		}
	} else {
		return (*itemIter).second.get();
	}
}

}
