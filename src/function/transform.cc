#include "transform.h"

#include <xalanc/XercesParserLiaison/FormatterToXercesDOM.hpp>

#include "transformer_facade.h"
#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"
#include "support/error/error_capacitor.h"

namespace InputXSLT {

DomDocumentCache::document_ptr FunctionTransform::constructDocument(
	const FilesystemContext&,
	xalan::XSLTInputSource   inputSource,
	xalan::XSLTInputSource   transformationSource
) const {
	DomDocumentCache::document_ptr domDocument(
		DomDocumentCache::createDocument("content")
	);

	ResultNodeFacade result(domDocument.get(), "transformation");
	TransformerFacade transformer(this->include_resolver_);

	try {
		xalan::FormatterToXercesDOM targetFormatter(
			domDocument.get(),
			result.getResultElement()
		);

		transformer.generate(
			inputSource,
			transformationSource,
			targetFormatter
		);

		result.setAttribute("result", "success");
	}
	catch (const ErrorCapacitor::exception& exception) {
		result.setAttribute("result", "error");

		for ( auto&& error : *exception ) {
			result.setValueNode("error", error);
		}
	}

	WarningCapacitor::warning_cache_ptr warnings(
		transformer.getCachedWarnings()
	);

	for ( auto&& warning : *warnings ) {
		result.setValueNode("warning", warning);
	}

	return domDocument;
}

}
