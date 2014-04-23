#include "read_directory.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/util/XMLString.hpp>

namespace InputXSLT {

FunctionReadDirectory::FunctionReadDirectory(const FilesystemContext& context):
	fs_context_(context),
	documents_(new std::stack<DomDocumentGuard>()) { }

xalan::XObjectPtr FunctionReadDirectory::execute(
	xalan::XPathExecutionContext&                executionContext,
	xalan::XalanNode*                            context,
	const xalan::Function::XObjectArgVectorType& arguments,
	const xalan::Locator*                        locator
) const {
	if ( arguments.size() != 1 ) {
		xalan::XPathExecutionContext::GetAndReleaseCachedString guard(
			executionContext
		);

		this->generalError(executionContext, context, locator);
	}

	this->documents_->emplace("content");
	DomDocumentGuard& domDocument = this->documents_->top();

	xercesc::DOMNode* const rootNode = domDocument->getDocumentElement();

	this->fs_context_.iterate(
		arguments[0]->str(),
		[&domDocument, &rootNode](const boost::filesystem::path& p) {
		XMLCh* buffer = xercesc::XMLString::transcode("file");
		xercesc::DOMNode* const fileNode = domDocument->createElement(buffer);
		xercesc::XMLString::release(&buffer);

		buffer = xercesc::XMLString::transcode(p.filename().string().data());
		xercesc::DOMText* const textNode = domDocument->createTextNode(buffer);
		xercesc::XMLString::release(&buffer);

		fileNode->appendChild(textNode);
		rootNode->appendChild(fileNode);
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
