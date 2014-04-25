#include "read_directory.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/util/XMLString.hpp>

#include "support/xerces_string_guard.h"

namespace InputXSLT {

FunctionReadDirectory::FunctionReadDirectory(const FilesystemContext& context):
	fs_context_(context),
	documents_(std::make_shared<std::stack<DomDocumentGuard>>()) { }

xalan::XObjectPtr FunctionReadDirectory::execute(
	xalan::XPathExecutionContext& executionContext,
	xalan::XalanNode*,
	const xalan::XObjectPtr argument,
	const xalan::Locator*
) const {
	this->documents_->emplace("content");
	DomDocumentGuard& domDocument = this->documents_->top();

	xercesc::DOMNode* const rootNode = domDocument->getDocumentElement();

	this->fs_context_.iterate(
		argument->str(),
		[&domDocument, &rootNode](const boost::filesystem::path& p) {
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
		rootNode->appendChild(itemNode);
	});

	return executionContext.getXObjectFactory().createNodeSet(
		domDocument.finalize()
	);
}

FunctionReadDirectory* FunctionReadDirectory::clone(
	xalan::MemoryManager& manager) const {
	return xalan::XalanCopyConstruct(manager, *this);
}

const xalan::XalanDOMString& FunctionReadDirectory::getError(
	xalan::XalanDOMString& result) const {
	result.assign("The read-directory() function expects one argument.");

	return result;
}

}
