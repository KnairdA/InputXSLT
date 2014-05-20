#include "transform.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include "transformation_facade.h"
#include "support/xerces_string_guard.h"

namespace InputXSLT {

FunctionTransform::FunctionTransform(IncludeEntityResolver* resolver):
	FunctionBase::FunctionBase(),
	include_resolver_(resolver) { }

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

	InputXSLT::TransformationFacade transformation(
		transformationPath,
		this->include_resolver_
	);

	if ( transformation.generate(targetPath, parameterObject) == 0 ) {
		xercesc::DOMElement* const resultNode(
			domDocument->createElement(*XercesStringGuard<XMLCh>("result"))
		);

		resultNode->setAttribute(
			*XercesStringGuard<XMLCh>("name"),
			*XercesStringGuard<XMLCh>(targetPath)
		);

		rootNode->appendChild(resultNode);
	} else {
		xercesc::DOMElement* const resultNode(
			domDocument->createElement(*XercesStringGuard<XMLCh>("error"))
		);

		rootNode->appendChild(resultNode);
	}

	return domDocument;
}

}
