#include "read_file.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>

#include "boost/filesystem/fstream.hpp"

#include "support/xerces_string_guard.h"

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

		xercesc::DOMText* const resultTextNode(
			domDocument->createTextNode(
				*XercesStringGuard<XMLCh>(readFile(filePath))
			)
		);

		resultNode->appendChild(resultTextNode);
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
