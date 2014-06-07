#include "external_text_formatter.h"

#include <xalanc/XSLT/XSLTInputSource.hpp>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
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

xercesc::DOMDocument* FunctionExternalTextFormatter::constructDocument(
	const InputXSLT::FilesystemContext&,
	std::string formatterPath,
	std::string stdinText
) {
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

	boost::process::context context;
	context.stdout_behavior = boost::process::capture_stream();
	context.stdin_behavior  = boost::process::capture_stream();

	boost::process::child formatterProcess(
		boost::process::launch(
			formatterPath,
			std::vector<std::string>{""},
			context
		)
	);

	boost::process::postream& inputStream  = formatterProcess.get_stdin();
	boost::process::pistream& outputStream = formatterProcess.get_stdout();

	inputStream << stdinText;
	inputStream.close();

	boost::process::status status = formatterProcess.wait();

	ResultNodeFacade result(domDocument, rootNode, "output");
	result.setAttribute("formatter", formatterPath);
	result.setAttribute("code",      std::to_string(status.exit_status()));

	if ( status.exited() ) {
		result.setAttribute("result", "success");
		result.setContent(
			importDocumentElement(outputStream, domDocument)->getChildNodes()
		);
	} else {
		result.setAttribute("result", "error");
	}


	return domDocument;
}

}
