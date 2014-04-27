#include "read_directory.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>

#include "support/utility.h"
#include "support/xerces_string_guard.h"

namespace InputXSLT {

FunctionReadDirectory::FunctionReadDirectory(const FilesystemContext& context):
	fs_context_(context),
	document_cache_(std::make_shared<DomDocumentCache>()) { }

xalan::XObjectPtr FunctionReadDirectory::execute(
	xalan::XPathExecutionContext& executionContext,
	xalan::XalanNode*,
	const xalan::XObjectPtr argument,
	const xalan::Locator*
) const {
	DomDocumentCache::item* const cachedDocument(
		this->document_cache_->get(xalanToString(argument->str()))
	);

	if ( !cachedDocument->isFinalized() ) {
		xercesc::DOMDocument* const domDocument(
			cachedDocument->getXercesDocument()
		);

		xercesc::DOMNode* const rootNode(
			domDocument->getDocumentElement()
		);

		if (boost::filesystem::is_directory(
			this->fs_context_.resolve(argument->str())
		)) {
			xercesc::DOMElement* const contentNode(
				domDocument->createElement(*XercesStringGuard("content"))
			);

			this->fs_context_.iterate(
				argument->str(),
				[&domDocument, &contentNode](const boost::filesystem::path& p) {
				xercesc::DOMElement* const itemNode(
					domDocument->createElement(*XercesStringGuard("item"))
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
						break;
					};
				}

				xercesc::DOMText* const textNode(
					domDocument->createTextNode(
						*XercesStringGuard(p.filename().string())
					)
				);

				itemNode->appendChild(textNode);
				contentNode->appendChild(itemNode);
			});

			xercesc::DOMElement* const resultNode(
				domDocument->createElement(*XercesStringGuard("status"))
			);

			xercesc::DOMText* const resultTextNode(
				domDocument->createTextNode(
					*XercesStringGuard("successful")
				)
			);

			resultNode->appendChild(resultTextNode);

			rootNode->appendChild(contentNode);
			rootNode->appendChild(resultNode);
		} else {
			xercesc::DOMElement* const resultNode(
				domDocument->createElement(*XercesStringGuard("status"))
			);

			xercesc::DOMText* const resultTextNode(
				domDocument->createTextNode(
					*XercesStringGuard("error")
				)
			);

			resultNode->appendChild(resultTextNode);
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
