#include "transform.h"

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
	const InputXSLT::FilesystemContext& fsContext,
	const FunctionBase::parameter_tuple& parameters
) {
	const std::string transformationPath(
		fsContext.resolve(std::get<0>(parameters)).string()
	);

	const std::string targetPath(
		fsContext.resolve(std::get<1>(parameters)).string()
	);

	const xalan::XObjectPtr& parameterObject(
		std::get<2>(parameters)
	);

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
	result.setAttribute("target", targetPath);

	if ( auto transformation = TransformationFacade::try_create(
		transformationPath,
		this->include_resolver_,
		handleErrors(result)
	) ) {
		try {
			transformation->generate(targetPath, parameterObject);

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
