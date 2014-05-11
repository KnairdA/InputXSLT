#ifndef INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_CACHE_H_
#define INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_CACHE_H_

#include <xalanc/XalanDOM/XalanDocument.hpp>

#include <xercesc/dom/DOMDocument.hpp>

#include <mutex>
#include <stack>
#include <memory>

#include "common.h"

namespace InputXSLT {

class DomDocumentCache {
	public:
		DomDocumentCache();

		xalan::XalanDocument* create(xercesc::DOMDocument*);

	private:
		class item;

		std::mutex write_mutex_;
		std::stack<std::unique_ptr<item>> cache_;

};

}

#include "document_cache_item.h"

#endif  // INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_CACHE_H_
