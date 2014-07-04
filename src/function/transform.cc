#include "transform.h"

#include <xalanc/XercesParserLiaison/FormatterToXercesDOM.hpp>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include "transformation_facade.h"
#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"
#include "support/error/error_capacitor.h"

namespace {

using InputXSLT::ErrorCapacitor;

inline std::function<void(const ErrorCapacitor::error_cache&)> handleErrors(
	InputXSLT::ResultNodeFacade& result) {
	return [&result](const ErrorCapacitor::error_cache& errors) {
		result.setAttribute("result", "error");

		for ( auto&& error : errors ) {
			result.setValueNode("error", error);
		}
	};
}

}

namespace InputXSLT {

xercesc::DOMDocument* FunctionTransform::constructDocument(
	xalan::XSLTInputSource  inputSource,
	xalan::XSLTInputSource  transformationSource,
	xalan::XObjectPtr       parameterObject
) {
	xercesc::DOMDocument* const domDocument(
		xercesc::DOMImplementation::getImplementation()->createDocument(
			nullptr,
			*XercesStringGuard<XMLCh>("content"),
			nullptr
		)
	);

	xercesc::DOMElement* const rootElement(
		domDocument->getDocumentElement()
	);

	ResultNodeFacade result(domDocument, rootElement, "transformation");

	if ( auto transformation = TransformationFacade::try_create(
		handleErrors(result),
		inputSource,
		transformationSource,
		this->include_resolver_
	) ) {
		try {
			xalan::FormatterToXercesDOM formatter(
				domDocument,
				result.getResultElement()
			);

			transformation->generate(
				formatter,
				parameterObject
			);

			result.setAttribute("result", "success");
		}
		catch (const ErrorCapacitor::exception& exception) {
			handleErrors(result)(*exception);
		}

		WarningCapacitor::warning_cache_ptr warnings(
			transformation->getCachedWarnings()
		);

		for ( auto&& warning : *warnings ) {
			result.setValueNode("warning", warning);
		}
	}

	return domDocument;
}

}
