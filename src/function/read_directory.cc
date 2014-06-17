#include "read_directory.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"

namespace InputXSLT {

xercesc::DOMDocument* FunctionReadDirectory::constructDocument(
	boost::filesystem::path directoryPath) {
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

	ResultNodeFacade result(domDocument, rootNode, "directory");
	result.setAttribute("path", directoryPath.string());

	if ( boost::filesystem::is_directory(directoryPath) ) {
		result.setAttribute("result", "success");

		xercesc::DOMNode* const resultNode = result.getNode();

		FilesystemContext::iterate(
			directoryPath,
			[&domDocument, &resultNode](const boost::filesystem::path& p) {
			ResultNodeFacade result(domDocument, resultNode, "entry");

			switch ( boost::filesystem::status(p).type() ) {
				case boost::filesystem::regular_file: {
					result.setAttribute("type",      "file");
					result.setValueNode("name",      p.stem().string());
					result.setValueNode("extension", p.extension().string());

					break;
				};
				case boost::filesystem::directory_file: {
					result.setAttribute("type", "directory");
					result.setValueNode("name", p.filename().string());

					break;
				};
				default: {
					result.setAttribute("type", "misc");
					result.setValueNode("name", p.filename().string());

					break;
				};
			}

			result.setValueNode("full", boost::filesystem::canonical(p).string());
		});
	} else {
		result.setAttribute("result", "error");
	}

	return domDocument;
}

}
