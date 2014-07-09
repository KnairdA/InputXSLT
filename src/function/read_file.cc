#include "read_file.h"

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>

#include <boost/optional.hpp>
#include <boost/filesystem/fstream.hpp>

#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"

namespace {

inline bool isXmlFile(const boost::filesystem::path& filePath) {
	return filePath.extension() == ".xml" ||
	       filePath.extension() == ".xsl";
}

boost::optional<xercesc::DOMNode*> readXmlFile(
	const boost::filesystem::path& filePath,
	xercesc::DOMDocument* const domDocument
) {
	const xercesc::LocalFileInputSource file(
		*InputXSLT::XercesStringGuard<XMLCh>(filePath.string().data())
	);

	xercesc::XercesDOMParser parser;
	parser.parse(file);

	if ( parser.getErrorCount() == 0 ) {
		return boost::make_optional(
			domDocument->importNode(
				parser.getDocument()->getDocumentElement(),
				true
			)
		);
	} else {
		return boost::optional<xercesc::DOMNode*>();
	}
}

boost::optional<std::string> readPlainFile(
	const boost::filesystem::path& filePath) {
	boost::filesystem::ifstream file(filePath);

	if ( file.is_open() ) {
		return boost::make_optional(
			std::string(
				(std::istreambuf_iterator<char>(file)),
				(std::istreambuf_iterator<char>())
			)
		);
	} else {
		return boost::optional<std::string>();
	}
}

}

namespace InputXSLT {

DomDocumentCache::document_ptr FunctionReadFile::constructDocument(
	boost::filesystem::path filePath) {
	DomDocumentCache::document_ptr domDocument(
		DomDocumentCache::createDocument("content")
	);

	ResultNodeFacade result(domDocument.get(), "file");
	result.setAttribute("path", filePath.string());

	if ( boost::filesystem::is_regular_file(filePath) ) {
		try {
			if ( isXmlFile(filePath) ) {
				result.setAttribute("type", "xml");

				if ( auto importedNode = readXmlFile(
					filePath,
					domDocument.get()) 
				) {
					result.setContent(*importedNode);

					result.setAttribute("result", "success");
				} else {
					result.setAttribute("result", "error");
				}
			} else {
				result.setAttribute("type", "plain");

				if ( auto plainContent = readPlainFile(filePath) ) {
					result.setContent(*plainContent);

					result.setAttribute("result", "success");
				} else {
					result.setAttribute("result", "error");
				}

				result.setAttribute("result", "success");
			}
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
