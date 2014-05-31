#include "read_file.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include "boost/filesystem/fstream.hpp"

#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"

namespace {

inline std::string readFile(const boost::filesystem::path& filePath) {
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

	ResultNodeFacade result(domDocument, rootNode, "file");
	result.setAttribute("path", filePath.string());

	if ( boost::filesystem::is_regular_file(filePath) ) {
		result.setAttribute("result", "success");

		result.setContent(readFile(filePath));
	} else {
		result.setAttribute("result", "success");
	}

	return domDocument;
}

}
