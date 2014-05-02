#include "read_directory.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>

#include "support/xerces_string_guard.h"
#include "support/filesystem_context.h"

namespace InputXSLT {

FunctionReadDirectory::FunctionReadDirectory():
	document_cache_(std::make_shared<DomDocumentCache>()) { }

xalan::XObjectPtr FunctionReadDirectory::execute(
	xalan::XPathExecutionContext& executionContext,
	xalan::XalanNode*,
	const xalan::XObjectPtr argument,
	const xalan::Locator* locator
) const {
	const FilesystemContext fs_context(
		boost::filesystem::path(
			xercesc::XMLString::transcode(locator->getSystemId() + 7)
		).parent_path().string()
	);

	const boost::filesystem::path directoryPath(
		fs_context.resolve(argument->str())
	);

	DomDocumentCache::item* const cachedDocument(
		this->document_cache_->get(directoryPath.string())
	);

	if ( !cachedDocument->isFinalized() ) {
		xercesc::DOMDocument* const domDocument(
			cachedDocument->getXercesDocument()
		);

		xercesc::DOMNode* const rootNode(
			domDocument->getDocumentElement()
		);

		if ( boost::filesystem::is_directory(directoryPath) ) {
			fs_context.iterate(
				argument->str(),
				[&domDocument, &rootNode](const boost::filesystem::path& p) {
				xercesc::DOMElement* const itemNode(
					domDocument->createElement(*XercesStringGuard("result"))
				);

				switch ( boost::filesystem::status(p).type() ) {
					case boost::filesystem::regular_file: {
						itemNode->setAttribute(
							*XercesStringGuard("type"),
							*XercesStringGuard("file")
						);

						break;
					};
					case boost::filesystem::directory_file: {
						itemNode->setAttribute(
							*XercesStringGuard("type"),
							*XercesStringGuard("directory")
						);

						break;
					};
					default: {
						itemNode->setAttribute(
							*XercesStringGuard("type"),
							*XercesStringGuard("misc")
						);

						break;
					};
				}

				xercesc::DOMText* const textNode(
					domDocument->createTextNode(
						*XercesStringGuard(p.filename().string())
					)
				);

				itemNode->appendChild(textNode);
				rootNode->appendChild(itemNode);
			});
		} else {
			xercesc::DOMElement* const resultNode(
				domDocument->createElement(*XercesStringGuard("error"))
			);

			rootNode->appendChild(resultNode);
		}
	}

	xalan::XPathExecutionContext::BorrowReturnMutableNodeRefList nodeList(
		executionContext
	);

	nodeList->addNodes(
		*cachedDocument->getXalanDocument()->getDocumentElement()->getChildNodes()
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
