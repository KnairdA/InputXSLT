#include "read_file.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>

#include "boost/filesystem/fstream.hpp"

#include "support/xerces_string_guard.h"
#include "support/filesystem_context.h"

namespace {

using InputXSLT::XercesStringGuard;

inline std::string readFile(const boost::filesystem::path& filePath) {
	boost::filesystem::ifstream file(filePath);

	return std::string(
		(std::istreambuf_iterator<char>(file)),
		(std::istreambuf_iterator<char>())
	);
}

xercesc::DOMDocument* constructDocument(
	const boost::filesystem::path& filePath
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

	if ( boost::filesystem::is_regular_file(filePath) ) {
		xercesc::DOMElement* const resultNode(
			domDocument->createElement(*XercesStringGuard<XMLCh>("result"))
		);

		resultNode->setAttribute(
			*XercesStringGuard<XMLCh>("name"),
			*XercesStringGuard<XMLCh>(filePath.filename().string())
		);

		xercesc::DOMText* const resultTextNode(
			domDocument->createTextNode(
				*XercesStringGuard<XMLCh>(readFile(filePath))
			)
		);

		resultNode->appendChild(resultTextNode);
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

FunctionReadFile::FunctionReadFile():
	document_cache_(std::make_shared<DomDocumentCache>()) { }

xalan::XObjectPtr FunctionReadFile::execute(
	xalan::XPathExecutionContext& executionContext,
	xalan::XalanNode*,
	const xalan::XObjectPtr argument,
	const xalan::Locator* locator
) const {
	const FilesystemContext fsContext(locator);

	const boost::filesystem::path filePath(
		fsContext.resolve(argument->str())
	);

	DomDocumentCache::optional_item optionalCachedDocument(
		this->document_cache_->get(filePath.string())
	);

	if ( !optionalCachedDocument.first ) {
		optionalCachedDocument = this->document_cache_->create(
			filePath.string(),
			constructDocument(filePath)
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
