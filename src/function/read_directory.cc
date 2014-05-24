#include "read_directory.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"

namespace InputXSLT {

xercesc::DOMDocument* FunctionReadDirectory::constructDocument(
	const InputXSLT::FilesystemContext& fsContext,
	const FunctionBase::parameter_tuple& parameters
) {
	const boost::filesystem::path directoryPath(
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

	if ( boost::filesystem::is_directory(directoryPath) ) {
		fsContext.iterate(
			directoryPath,
			[&domDocument, &rootNode](const boost::filesystem::path& p) {
			ResultNodeFacade result(domDocument, rootNode, "result");

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
		ResultNodeFacade result(domDocument, rootNode, "error");
	}

	return domDocument;
}

}
