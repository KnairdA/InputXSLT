#include "external_command.h"

#include <xalanc/XSLT/XSLTInputSource.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>

#include <boost/process.hpp>
#include <boost/optional.hpp>

#include <sstream>

#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"

namespace {

using InputXSLT::XercesStringGuard;

inline bool isWrappedOutput(const XMLCh* nodeName) {
	return xercesc::XMLString::equals(
		nodeName,
		*XercesStringGuard<XMLCh>("inputxslt_wrapper")
	);
}

std::unique_ptr<std::stringstream> readOutput(
	boost::process::ipstream& outputStream) {
	const std::string rawOutput(
		(std::istreambuf_iterator<char>(outputStream)),
		(std::istreambuf_iterator<char>())
	);

	if (    rawOutput.substr(0, 5) == "<?xml"
	     || rawOutput.substr(0,2) == "<!" ) {
		return std::make_unique<std::stringstream>(
			rawOutput
		);
	} else {
		return std::make_unique<std::stringstream>(
			"<inputxslt_wrapper>" + rawOutput + "</inputxslt_wrapper>"
		);
	}
}

boost::optional<xercesc::DOMNode*> importDocumentElement(
	std::stringstream*    const outputStream,
	xercesc::DOMDocument* const domDocument
) {
	xercesc::XercesDOMParser parser;
	parser.setDoNamespaces(true);
	parser.parse(xalan::XSLTInputSource(*outputStream));

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

}

namespace InputXSLT {

DomDocumentCache::document_ptr FunctionExternalCommand::constructDocument(
	const FilesystemContext&     fsContext,
	std::string                  command,
	boost::optional<std::string> input
) const {
	namespace bp = boost::process;

	DomDocumentCache::document_ptr domDocument{
		DomDocumentCache::createDocument("content")};
	ResultNodeFacade result(domDocument.get(), "command");

	try {
		bp::ipstream commandOutput;
		bp::opstream commandInput;

		bp::child commandProcess{
			command,
			bp::start_dir = boost::filesystem::canonical(fsContext.getBase().parent_path()),
			bp::std_out   = commandOutput,
			bp::std_in    = commandInput
		};

		if ( input ) {
			commandInput << *input;
			commandInput.flush();
		}
		commandInput.pipe().close();

		std::unique_ptr<std::stringstream> outputStream{
			readOutput(commandOutput)};
		commandProcess.wait();

		result.setAttribute("executed", command);
		result.setAttribute("code",     std::to_string(commandProcess.exit_code()));

		if ( boost::optional<xercesc::DOMNode*> documentElement{
			importDocumentElement(outputStream.get(), domDocument.get())
		} ) {
			if ( isWrappedOutput((*documentElement)->getNodeName()) ) {
				result.setContent(
					(*documentElement)->getChildNodes()
				);
			} else {
				result.setContent(*documentElement);
			}

			result.setAttribute("result", "success");
		} else {
			result.setAttribute("result", "error");
		}
	}
	catch ( const std::system_error& exception ) {
		result.setAttribute("result", "error");
	}
	catch ( const xercesc::DOMException& exception ) {
		result.setAttribute("result", "error");

		result.setValueNode(
			"error",
			*XercesStringGuard<char>(exception.msg)
		);
	}

	return domDocument;
}

}
