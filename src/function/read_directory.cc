#include "read_directory.h"

#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/util/XMLString.hpp>

#include <sstream>

namespace InputXSLT {

FunctionReadDirectory::FunctionReadDirectory(const FilesystemContext& context):
	fs_context_(context),
	parser_() { }

FunctionReadDirectory::FunctionReadDirectory(const FunctionReadDirectory& src):
	fs_context_(src.fs_context_),
	parser_() { }

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

	xercesc::DOMDocument* const inputDom(
		xercesc::DOMImplementation::getImplementation()->createDocument(
			nullptr, xercesc::XMLString::transcode("content"), nullptr
		)
	);

	xercesc::DOMNode* const rootNode = inputDom->getDocumentElement();

	this->fs_context_.iterate(
		arguments[0]->str(),
		[&inputDom, &rootNode](const boost::filesystem::path& p) {
		xercesc::DOMNode* const fileNode = inputDom->createElement(
			xercesc::XMLString::transcode("file")
		);

		xercesc::DOMText* const textNode = inputDom->createTextNode(
			xercesc::XMLString::transcode(p.filename().string().data())
		);

		fileNode->appendChild(textNode);
		rootNode->appendChild(fileNode);
	});


	xalan::XercesDOMSupport domSupport(this->parser_);

	xalan::XercesDOMWrapperParsedSource* const parsedSource(
		new xalan::XercesDOMWrapperParsedSource(
			inputDom,
			this->parser_,
			domSupport
		)
	);

	return executionContext.getXObjectFactory().createNodeSet(
		parsedSource->getDocument()
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
