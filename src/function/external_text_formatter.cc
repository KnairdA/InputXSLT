#include "external_text_formatter.h"

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

DomDocumentCache::document_ptr FunctionExternalTextFormatter::constructDocument(
	std::string formatterCommand,
	std::string stdinText
) const {
	DomDocumentCache::document_ptr domDocument(
		DomDocumentCache::createDocument("content")
	);

	boost::process::context context;
	context.environment     = boost::process::self::get_environment(); 
	context.stdout_behavior = boost::process::capture_stream();
	context.stdin_behavior  = boost::process::capture_stream();

	boost::process::child formatterProcess(
		boost::process::launch_shell(
			formatterCommand,
			context
		)
	);

	boost::process::postream& inputStream  = formatterProcess.get_stdin();
	boost::process::pistream& outputStream = formatterProcess.get_stdout();

	inputStream << stdinText;
	inputStream.close();

	boost::process::status status = formatterProcess.wait();

	ResultNodeFacade result(domDocument.get(), "output");
	result.setAttribute("formatter", formatterCommand);
	result.setAttribute("code",      std::to_string(status.exit_status()));

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
