#include "read_xml_file.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

#include "boost/filesystem/fstream.hpp"

#include "support/xerces_string_guard.h"
#include "support/filesystem_context.h"

namespace {

inline xercesc::DOMNode* importDocumentElement(
	const boost::filesystem::path& filePath,
	xercesc::DOMDocument* const domDocument
) {
	xercesc::XercesDOMParser parser;
	boost::filesystem::ifstream file(filePath);
	parser.parse(xalan::XSLTInputSource(file));

	return domDocument->importNode(
		parser.getDocument()->getDocumentElement(),
		true
	);
}

}

namespace InputXSLT {

FunctionReadXmlFile::FunctionReadXmlFile():
	document_cache_(std::make_shared<DomDocumentCache>()) { }

xalan::XObjectPtr FunctionReadXmlFile::execute(
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

	const boost::filesystem::path filePath(
		fs_context.resolve(argument->str())
	);

	DomDocumentCache::item* const cachedDocument(
		this->document_cache_->get(filePath.string())
	);

	if ( !cachedDocument->isFinalized() ) {
		xercesc::DOMDocument* const domDocument(
			cachedDocument->getXercesDocument()
		);

		xercesc::DOMNode* const rootNode(
			domDocument->getDocumentElement()
		);

		if ( boost::filesystem::is_regular_file(filePath) ) {
			xercesc::DOMElement* const resultNode(
				domDocument->createElement(*XercesStringGuard("result"))
			);

			resultNode->setAttribute(
				*XercesStringGuard("name"),
				*XercesStringGuard(filePath.filename().string())
			);

			xercesc::DOMNode* const resultTreeNode(
				importDocumentElement(filePath, domDocument)
			);

			resultNode->appendChild(resultTreeNode);
			rootNode->appendChild(resultNode);
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

FunctionReadXmlFile* FunctionReadXmlFile::clone(
	xalan::MemoryManager& manager) const {
	return xalan::XalanCopyConstruct(manager, *this);
}

const xalan::XalanDOMString& FunctionReadXmlFile::getError(
	xalan::XalanDOMString& result) const {
	result.assign("The read-xml-file() function expects one argument of type string.");

	return result;
}

}
