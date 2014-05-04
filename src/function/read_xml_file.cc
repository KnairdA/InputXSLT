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

using InputXSLT::XercesStringGuard;

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

		xercesc::DOMNode* const resultTreeNode(
			importDocumentElement(filePath, domDocument)
		);

		resultNode->appendChild(resultTreeNode);
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

FunctionReadXmlFile::FunctionReadXmlFile():
	document_cache_(std::make_shared<DomDocumentCache>()) { }

xalan::XObjectPtr FunctionReadXmlFile::execute(
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
