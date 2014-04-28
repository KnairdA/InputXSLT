#ifndef INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_CACHE_ITEM_H_
#define INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_CACHE_ITEM_H_

#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>

#include <xercesc/dom/DOMDocument.hpp>

#include <string>
#include <memory>

#include "common.h"
#include "document_cache.h"

namespace InputXSLT {

class DomDocumentCache::item {
	public:
		~item();

		bool isFinalized() const;

		xercesc::DOMDocument* getXercesDocument() const;
		xalan::XalanDocument* getXalanDocument();

	protected:
		friend DomDocumentCache;

		item(const std::string&);

	private:
		xalan::XercesParserLiaison parser_;
		xalan::XercesDOMSupport dom_support_;
		xercesc::DOMDocument* const document_;
		std::unique_ptr<xalan::XercesDOMWrapperParsedSource> parsed_source_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_CACHE_ITEM_H_