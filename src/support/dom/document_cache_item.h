#ifndef INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_CACHE_ITEM_H_
#define INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_CACHE_ITEM_H_

#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>

#include <xercesc/dom/DOMDocument.hpp>

#include "common.h"
#include "document_cache.h"

namespace InputXSLT {

class DomDocumentCache::item {
	public:
		xalan::XalanDocument* getXalanDocument();

	protected:
		friend std::unique_ptr<item> std::make_unique<item>(document_ptr&&);

		item(document_ptr&&);

	private:
		document_ptr document_;
		xalan::XercesParserLiaison parser_;
		xalan::XercesDOMSupport dom_support_;
		xalan::XercesDOMWrapperParsedSource parsed_source_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_CACHE_ITEM_H_
