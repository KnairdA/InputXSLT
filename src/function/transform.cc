#include "transform.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include <boost/algorithm/string.hpp>

#include "transformation_facade.h"
#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"

namespace InputXSLT {

xercesc::DOMDocument* FunctionTransform::constructDocument(
	const InputXSLT::FilesystemContext& fsContext,
	const FunctionBase::parameter_tuple& parameters
) {
	const std::string transformationPath(boost::trim_copy(
		fsContext.resolve(std::get<0>(parameters)).string()
	));

	const std::string targetPath(boost::trim_copy(
		fsContext.resolve(std::get<1>(parameters)).string()
	));

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
		ResultNodeFacade result(domDocument, rootNode, "result");

		result.setAttribute("name", targetPath);
	} else {
		ResultNodeFacade result(domDocument, rootNode, "error");
	}

	return domDocument;
}

}
