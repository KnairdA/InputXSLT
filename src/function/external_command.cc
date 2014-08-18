#include "external_command.h"

#include <xalanc/XSLT/XSLTInputSource.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>

#include <boost/process.hpp>

#include <sstream>

#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"

namespace {

using InputXSLT::XercesStringGuard;

inline xercesc::DOMNode* importDocumentElement(
	boost::process::pistream& outputStream,
	xercesc::DOMDocument* const domDocument
) {
	std::stringstream xmlStream(
		"<output>" + std::string(
			(std::istreambuf_iterator<char>(outputStream)),
			(std::istreambuf_iterator<char>())
		) + "</output>"
	);

	xercesc::XercesDOMParser parser;
	parser.parse(xalan::XSLTInputSource(xmlStream));

	return domDocument->importNode(
		parser.getDocument()->getDocumentElement(),
		true
	);
}

}

namespace InputXSLT {

DomDocumentCache::document_ptr FunctionExternalCommand::constructDocument(
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

	boost::process::child commandProcess(
		boost::process::launch_shell(command, context)
	);

	if ( input ) {
		boost::process::postream& inputStream = commandProcess.get_stdin();
		inputStream << *input;
		inputStream.close();
	}

	boost::process::pistream& outputStream = commandProcess.get_stdout();
	boost::process::status status          = commandProcess.wait();

	ResultNodeFacade result(domDocument.get(), "command");
	result.setAttribute("executed", command);
	result.setAttribute("code",     std::to_string(status.exit_status()));

	if ( status.exited() ) {
		try {
			result.setContent(
				importDocumentElement(
					outputStream,
					domDocument.get()
				)->getChildNodes()
			);

			result.setAttribute("result", "success");
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
