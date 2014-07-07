#include "write_file.h"

#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanStdOutputStream.hpp>
#include <xalanc/XMLSupport/FormatterToXML.hpp>
#include <xalanc/XMLSupport/FormatterTreeWalker.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "support/xalan_string.h"
#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"

namespace {

bool serializeNodeToFile(
	const boost::filesystem::path& filePath,
	xalan::XalanNode* const        contentNode
) {
	const xalan::XalanNode::NodeType contentType(
		contentNode->getNodeType()
	);

	if ( contentType != xalan::XalanNode::DOCUMENT_NODE &&
	     contentType != xalan::XalanNode::ATTRIBUTE_NODE ) {
		boost::filesystem::ofstream file(filePath);

		if ( file.is_open() ) {
			if ( contentType == xalan::XalanNode::TEXT_NODE ) {
				file << InputXSLT::toString(contentNode->getNodeValue());
			} else {
				xalan::XalanStdOutputStream         outputStream(file);
				xalan::XalanOutputStreamPrintWriter outputWriter(outputStream);
				xalan::FormatterToXML               formatter(outputWriter);
				xalan::FormatterTreeWalker          walker(formatter);

				formatter.startDocument();

				walker.traverseSubtree(contentNode);

				formatter.endDocument();
			}

			file << std::endl;

			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

}

namespace InputXSLT {

DomDocumentCache::document_ptr FunctionWriteFile::constructDocument(
	boost::filesystem::path filePath,
	xalan::XalanNode* const contentNode
) {
	DomDocumentCache::document_ptr domDocument(
		DomDocumentCache::createDocument()
	);

	xercesc::DOMNode* const rootNode(
		domDocument->getDocumentElement()
	);

	ResultNodeFacade result(domDocument.get(), rootNode, "file");
	result.setAttribute("path", filePath.string());

	if ( contentNode != nullptr ) {
		if ( serializeNodeToFile(filePath, contentNode) ) {
			result.setAttribute("result", "success");
		} else {
			result.setAttribute("result", "error");
		}
	} else {
		result.setAttribute("result", "error");
	}

	return domDocument;
}

}
