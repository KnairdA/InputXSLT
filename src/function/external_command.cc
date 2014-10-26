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

inline std::string wrapOutput(const std::string& rawOutput) {
	return std::string(
		"<inputxslt_wrapper>" + rawOutput + "</inputxslt_wrapper>"
	);
}

inline bool isWrappedOutput(const XMLCh* nodeName) {
	return xercesc::XMLString::equals(
		nodeName,
		*XercesStringGuard<XMLCh>("inputxslt_wrapper")
	);
}

inline std::unique_ptr<std::stringstream> readOutput(
	boost::process::pistream& outputStream) {
	const std::string rawOutput(
		(std::istreambuf_iterator<char>(outputStream)),
		(std::istreambuf_iterator<char>())
	);

	if ( rawOutput.substr(0, 5) == "<?xml" ) {
		return std::make_unique<std::stringstream>(
			rawOutput
		);
	} else {
		return std::make_unique<std::stringstream>(
			wrapOutput(rawOutput)
		);
	}
}

inline boost::optional<xercesc::DOMNode*> importDocumentElement(
	std::stringstream*    const outputStream,
	xercesc::DOMDocument* const domDocument
) {
	xercesc::XercesDOMParser parser;
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
	DomDocumentCache::document_ptr domDocument(
		DomDocumentCache::createDocument("content")
	);

	boost::process::context context;
	context.environment     = boost::process::self::get_environment(); 
	context.stdout_behavior = boost::process::capture_stream();
	context.stdin_behavior  = boost::process::capture_stream();
	context.work_directory  = boost::filesystem::canonical(
		fsContext.getBase().parent_path()
	).string();

	boost::process::child commandProcess(
		boost::process::launch_shell(command, context)
	);

	if ( input ) {
		boost::process::postream& inputStream = commandProcess.get_stdin();
		inputStream << *input;
		inputStream.close();
	}

	std::unique_ptr<std::stringstream> outputStream{
		readOutput(commandProcess.get_stdout())
	};
	boost::process::status status = commandProcess.wait();

	ResultNodeFacade result(domDocument.get(), "command");
	result.setAttribute("executed", command);
	result.setAttribute("code",     std::to_string(status.exit_status()));

	if ( status.exited() ) {
		try {
			if ( boost::optional<xercesc::DOMNode*> documentElement{
				importDocumentElement(
					outputStream.get(),
					domDocument.get()
				)
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
