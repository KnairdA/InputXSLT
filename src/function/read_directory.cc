#include "read_directory.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>

#include "support/xerces_string_guard.h"
#include "support/filesystem_context.h"

namespace {

using InputXSLT::XercesStringGuard;

xercesc::DOMDocument* constructDocument(
	const InputXSLT::FilesystemContext& fsContext,
	const boost::filesystem::path& directoryPath
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

	if ( boost::filesystem::is_directory(directoryPath) ) {
		fsContext.iterate(
			directoryPath,
			[&domDocument, &rootNode](const boost::filesystem::path& p) {
			xercesc::DOMElement* const itemNode(
				domDocument->createElement(*XercesStringGuard<XMLCh>("result"))
			);

			switch ( boost::filesystem::status(p).type() ) {
				case boost::filesystem::regular_file: {
					itemNode->setAttribute(
						*XercesStringGuard<XMLCh>("type"),
						*XercesStringGuard<XMLCh>("file")
					);

					break;
				};
				case boost::filesystem::directory_file: {
					itemNode->setAttribute(
						*XercesStringGuard<XMLCh>("type"),
						*XercesStringGuard<XMLCh>("directory")
					);

					break;
				};
				default: {
					itemNode->setAttribute(
						*XercesStringGuard<XMLCh>("type"),
						*XercesStringGuard<XMLCh>("misc")
					);

					break;
				};
			}

			xercesc::DOMText* const textNode(
				domDocument->createTextNode(
					*XercesStringGuard<XMLCh>(p.filename().string())
				)
			);

			itemNode->appendChild(textNode);
			rootNode->appendChild(itemNode);
		});
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

FunctionReadDirectory::FunctionReadDirectory():
	document_cache_(std::make_shared<DomDocumentCache>()) { }

xalan::XObjectPtr FunctionReadDirectory::execute(
	xalan::XPathExecutionContext& executionContext,
	xalan::XalanNode*,
	const xalan::XObjectPtr argument,
	const xalan::Locator* locator
) const {
	const FilesystemContext fsContext(locator);

	const boost::filesystem::path directoryPath(
		fsContext.resolve(argument->str())
	);

	DomDocumentCache::optional_item optionalCachedDocument(
		this->document_cache_->get(directoryPath.string())
	);

	if ( !optionalCachedDocument.first ) {
		optionalCachedDocument = this->document_cache_->create(
			directoryPath.string(),
			constructDocument(fsContext, directoryPath)
		);
	}

	xalan::XPathExecutionContext::BorrowReturnMutableNodeRefList nodeList(
		executionContext
	);

	nodeList->addNodes(
		*optionalCachedDocument.second->getXalanDocument()
		                              ->getDocumentElement()
		                              ->getChildNodes()
	);

	return executionContext.getXObjectFactory().createNodeSet(nodeList);
}

FunctionReadDirectory* FunctionReadDirectory::clone(
	xalan::MemoryManager& manager) const {
	return xalan::XalanCopyConstruct(manager, *this);
}

const xalan::XalanDOMString& FunctionReadDirectory::getError(
	xalan::XalanDOMString& result) const {
	result.assign("The read-directory() function expects one argument of type string.");

	return result;
}

}
