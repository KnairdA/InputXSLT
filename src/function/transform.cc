#include "transform.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include <sstream>

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

	xercesc::DOMNode* const rootNode(
		domDocument->getDocumentElement()
	);

	ResultNodeFacade result(domDocument, rootNode, "transformation");

	if ( auto transformation = TransformationFacade::try_create(
		handleErrors(result),
		transformationSource,
		this->include_resolver_
	) ) {
		try {
			std::stringstream targetStream;

			transformation->generate(
				targetStream,
				parameterObject
			);

			result.setContent(
				targetStream.str()
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
