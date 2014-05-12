#include "transform.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include "transformation_facade.h"
#include "support/xerces_string_guard.h"

namespace {

using InputXSLT::XercesStringGuard;

xercesc::DOMDocument* constructDocument(
	const InputXSLT::FilesystemContext&,
	const boost::filesystem::path& transformationPath,
	const boost::filesystem::path& targetPath
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

namespace InputXSLT {

FunctionTransform::FunctionTransform():
	document_cache_(std::make_shared<DomDocumentCache>()) { }

xalan::XObjectPtr FunctionTransform::execute(
	xalan::XPathExecutionContext& executionContext,
	xalan::XalanNode*,
	const xalan::XObjectPtr argument1,
	const xalan::XObjectPtr argument2,
	const xalan::Locator* locator
) const {
	const FilesystemContext fsContext(locator);

	xalan::XalanDocument* const domDocument(
		this->document_cache_->create(
			constructDocument(
				fsContext,
				fsContext.resolve(argument1->str()),
				fsContext.resolve(argument2->str())
			)
		)
	);

	xalan::XPathExecutionContext::BorrowReturnMutableNodeRefList nodeList(
		executionContext
	);

	nodeList->addNodes(
		*domDocument->getDocumentElement()->getChildNodes()
	);

	return executionContext.getXObjectFactory().createNodeSet(nodeList);
}

FunctionTransform* FunctionTransform::clone(
	xalan::MemoryManager& manager) const {
	return xalan::XalanCopyConstruct(
		manager,
		*this
	);
}

const xalan::XalanDOMString& FunctionTransform::getError(
	xalan::XalanDOMString& result) const {
	result.assign("The function expects two arguments of type string.");

	return result;
}

}
