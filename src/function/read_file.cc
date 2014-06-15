#include "read_file.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>

#include "boost/filesystem/fstream.hpp"

#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"

namespace {

inline bool isXmlFile(const boost::filesystem::path& filePath) {
	return filePath.extension() == ".xml";
}

inline xercesc::DOMNode* readXmlFile(
	const boost::filesystem::path& filePath,
	xercesc::DOMDocument* const domDocument
) {
	const xercesc::LocalFileInputSource file(
		*InputXSLT::XercesStringGuard<XMLCh>(filePath.string().data())
	);

	xercesc::XercesDOMParser parser;
	parser.parse(file);

	return domDocument->importNode(
		parser.getDocument()->getDocumentElement(),
		true
	);
}

inline std::string readPlainFile(const boost::filesystem::path& filePath) {
	boost::filesystem::ifstream file(filePath);

	return std::string(
		(std::istreambuf_iterator<char>(file)),
		(std::istreambuf_iterator<char>())
	);
}

}

namespace InputXSLT {

xercesc::DOMDocument* FunctionReadFile::constructDocument(
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
		try {
			if ( isXmlFile(filePath) ) {
				result.setAttribute("type", "xml");

				result.setContent(
					readXmlFile(filePath.string(), domDocument)
				);
			} else {
				result.setAttribute("type", "plain");

				result.setContent(
					readPlainFile(filePath)
				);
			}

			result.setAttribute("result", "success");
		}
		catch ( const xercesc::DOMException& exception ) {
			result.setAttribute("result", "error");

			result.setValueNode(
				"error",
				*XercesStringGuard<char>(exception.msg)
			);
		}
	} else {
		result.setAttribute("result", "error");
	}

	return domDocument;
}

}
