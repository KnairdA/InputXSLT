#include "read_file.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/util/XMLString.hpp>

#include "boost/filesystem/fstream.hpp"

#include <fstream>

#include "support/xerces_string_guard.h"
#include "support/utility.h"

namespace InputXSLT {

FunctionReadFile::FunctionReadFile(const FilesystemContext& context):
	fs_context_(context),
	document_cache_(std::make_shared<DomDocumentCache>()) { }

xalan::XObjectPtr FunctionReadFile::execute(
	xalan::XPathExecutionContext& executionContext,
	xalan::XalanNode*,
	const xalan::XObjectPtr argument,
	const xalan::Locator*
) const {
	const boost::filesystem::path filePath(
		this->fs_context_.resolve(argument->str())
	);

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

		if ( boost::filesystem::is_regular_file(filePath) ) {
			boost::filesystem::ifstream file(filePath);

			const std::string fileContent(
				(std::istreambuf_iterator<char>(file)),
				(std::istreambuf_iterator<char>())
			);

			xercesc::DOMElement* const contentNode(
				domDocument->createElement(*XercesStringGuard("content"))
			);

			xercesc::DOMText* const contentTextNode(
				domDocument->createTextNode(
					*XercesStringGuard(fileContent)
				)
			);

			xercesc::DOMElement* const resultNode(
				domDocument->createElement(*XercesStringGuard("status"))
			);

			xercesc::DOMText* const resultTextNode(
				domDocument->createTextNode(
					*XercesStringGuard("successful")
				)
			);

			contentNode->appendChild(contentTextNode);
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

FunctionReadFile* FunctionReadFile::clone(
	xalan::MemoryManager& manager) const {
	return xalan::XalanCopyConstruct(manager, *this);
}

const xalan::XalanDOMString& FunctionReadFile::getError(
	xalan::XalanDOMString& result) const {
	result.assign("The read-file() function expects one argument of type string.");

	return result;
}

}
