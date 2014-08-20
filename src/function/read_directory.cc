#include "read_directory.h"

#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"

namespace InputXSLT {

DomDocumentCache::document_ptr FunctionReadDirectory::constructDocument(
	const FilesystemContext&,
	boost::filesystem::path  directoryPath
) const {
	DomDocumentCache::document_ptr domDocument(
		DomDocumentCache::createDocument("content")
	);

	ResultNodeFacade result(domDocument.get(), "directory");
	result.setAttribute("path", directoryPath.string());

	if ( boost::filesystem::is_directory(directoryPath) ) {
		result.setAttribute("result", "success");

		xercesc::DOMNode* const resultNode = result.getResultElement();

		FilesystemContext::iterate(
			directoryPath,
			[&domDocument, &resultNode](const boost::filesystem::path& p) {
			ResultNodeFacade result(domDocument.get(), resultNode, "entry");

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
