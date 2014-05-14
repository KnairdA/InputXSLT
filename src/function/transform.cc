#include "transform.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include "transformation_facade.h"
#include "support/xerces_string_guard.h"

namespace InputXSLT {

xercesc::DOMDocument* FunctionTransform::constructDocument(
	const InputXSLT::FilesystemContext&,
	const FunctionBase::argument_array& arguments
) {
	const boost::filesystem::path& transformationPath = arguments[0];
	const boost::filesystem::path& targetPath         = arguments[1];

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
		transformationPath.string()
	);

	const int result = transformation.generate(
		targetPath.string()
	);

	if ( result == 0 ) {
		xercesc::DOMElement* const resultNode(
			domDocument->createElement(*XercesStringGuard<XMLCh>("result"))
		);

		resultNode->setAttribute(
			*XercesStringGuard<XMLCh>("name"),
			*XercesStringGuard<XMLCh>(targetPath.string())
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
