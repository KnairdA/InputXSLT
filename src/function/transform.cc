#include "transform.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include "transformation_facade.h"
#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"

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

	ResultNodeFacade result(domDocument, rootNode, "result");
	result.setAttribute("name", targetPath);

	InputXSLT::TransformationFacade transformation(
		transformationPath,
		this->include_resolver_
	);

	InputXSLT::TransformationFacade::return_type errors(
		transformation.generate(targetPath, parameterObject)
	);

	if ( errors ) {
		for ( auto&& error : *errors ) {
			result.setValueNode("error", error);
		}
	}

	return domDocument;
}

}
