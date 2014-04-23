#ifndef INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_GUARD_H_
#define INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_GUARD_H_

#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>

#include <xercesc/dom/DOMDocument.hpp>

#include <string>
#include <memory>

#include "common.h"

namespace InputXSLT {

class DomDocumentGuard {
	public:
		DomDocumentGuard(const std::string&);
		~DomDocumentGuard();

		xercesc::DOMDocument* operator->();
		xalan::XalanDocument* finalize();

	private:
		xalan::XercesParserLiaison parser_;
		xalan::XercesDOMSupport dom_support_;
		XMLCh* root_node_name_;
		xercesc::DOMDocument* const document_;
		std::unique_ptr<xalan::XercesDOMWrapperParsedSource> parsed_source_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_DOM_DOCUMENT_GUARD_H_
