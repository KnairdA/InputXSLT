#ifndef INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_CACHE_H_
#define INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_CACHE_H_

#include <map>
#include <string>
#include <memory>

namespace InputXSLT {

class DomDocumentCache {
	public:
		class item;

		DomDocumentCache();

		item* get(const std::string&);

	private:
		std::map<std::string, std::unique_ptr<item>> map_;

};

}

#include "document_cache_item.h"

#endif  // INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_CACHE_H_
