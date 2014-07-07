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
	class document_deleter {
		friend std::unique_ptr<xercesc::DOMDocument, document_deleter>;

		void operator()(xercesc::DOMDocument*);
	};

	public:
		typedef std::unique_ptr<
			xercesc::DOMDocument,
			document_deleter
		> document_ptr;

		static document_ptr createDocument();

		DomDocumentCache();

		xalan::XalanDocument* create(document_ptr&&);

	private:
		class item;

		std::mutex write_mutex_;
		std::stack<std::unique_ptr<item>> cache_;

};

}

#include "document_cache_item.h"

#endif  // INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_CACHE_H_
