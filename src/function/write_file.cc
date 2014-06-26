#include "write_file.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"

namespace {

bool writeFile(
	const boost::filesystem::path& filePath,
	const std::string&             content
) {
	boost::filesystem::ofstream file(filePath);

	if ( file.is_open() ) {
		file << content << std::endl;

		return true;
	} else {
		return false;
	}
}

}

namespace InputXSLT {

xercesc::DOMDocument* FunctionWriteFile::constructDocument(
	boost::filesystem::path filePath,
	std::string             content
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

	ResultNodeFacade result(domDocument, rootNode, "file");
	result.setAttribute("path", filePath.string());

	if ( writeFile(filePath, content) ) {
		result.setAttribute("result", "success");
	} else {
		result.setAttribute("result", "error");
	}

	return domDocument;
}

}
