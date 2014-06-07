#include "read_xml_file.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>

#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"

namespace {

inline xercesc::DOMNode* importDocumentElement(
	const std::string& filePath,
	xercesc::DOMDocument* const domDocument
) {
	const xercesc::LocalFileInputSource file(
		*InputXSLT::XercesStringGuard<XMLCh>(filePath.data())
	);

	xercesc::XercesDOMParser parser;
	parser.parse(file);

	return domDocument->importNode(
		parser.getDocument()->getDocumentElement(),
		true
	);
}

}

namespace InputXSLT {

xercesc::DOMDocument* FunctionReadXmlFile::constructDocument(
	const FilesystemContext& fsContext,
	std::string rawPath
) {
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

	ResultNodeFacade result(domDocument, rootNode, "file");
	result.setAttribute("path", filePath.string());

	if ( boost::filesystem::is_regular_file(filePath) ) {
		result.setAttribute("result", "success");

		result.setContent(
			importDocumentElement(filePath.string(), domDocument)
		);
	} else {
		result.setAttribute("result", "error");
	}

	return domDocument;
}

}
