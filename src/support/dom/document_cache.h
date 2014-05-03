#ifndef INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_CACHE_H_
#define INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_CACHE_H_

#include <xercesc/dom/DOMDocument.hpp>

#include <string>
#include <mutex>
#include <unordered_map>
#include <memory>

namespace InputXSLT {

class DomDocumentCache {
	public:
		class item;
		typedef std::pair<bool, item*> optional_item;

		DomDocumentCache();

		optional_item get(const std::string&);
		optional_item create(const std::string&, xercesc::DOMDocument*);

	private:
		std::mutex write_mutex_;
		std::unordered_map<std::string, std::unique_ptr<item>> map_;

};

}

#include "document_cache_item.h"

#endif  // INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_CACHE_H_
