#include "read_xml_file.h"

#include <xalanc/XSLT/XSLTInputSource.hpp>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

#include "boost/filesystem/fstream.hpp"

#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"

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
	const std::string& rawPath = std::get<0>(parameters);
	boost::filesystem::path filePath(fsContext.resolve(rawPath));

	if ( !(boost::filesystem::exists(filePath) &&
	       boost::filesystem::is_regular_file(filePath)) ) {
		if ( auto resolvedPath = this->include_resolver_->resolve(rawPath) ) {
			filePath = *resolvedPath;
		}
	}

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
		ResultNodeFacade result(domDocument, rootNode, "result");

		result.setAttribute("name", filePath.filename().string());

		result.setContent(
			importDocumentElement(filePath, domDocument)
		);
	} else {
		ResultNodeFacade result(domDocument, rootNode, "error");
	}

	return domDocument;
}

}
