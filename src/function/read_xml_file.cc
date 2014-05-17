#include "read_xml_file.h"

#include <xalanc/XSLT/XSLTInputSource.hpp>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

#include "boost/filesystem/fstream.hpp"

#include "support/xerces_string_guard.h"

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

}

namespace InputXSLT {

xercesc::DOMDocument* FunctionReadXmlFile::constructDocument(
	const FilesystemContext& fsContext,
	const FunctionBase::parameter_tuple& parameters
) {
	const boost::filesystem::path filePath(
		fsContext.resolve(std::get<0>(parameters))
	);

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
