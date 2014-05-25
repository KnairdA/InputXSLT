#include "execute.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include <boost/process.hpp>

#include "support/xerces_string_guard.h"
#include "support/dom/result_node_facade.h"

namespace InputXSLT {

xercesc::DOMDocument* FunctionExecute::constructDocument(
	const InputXSLT::FilesystemContext&,
	const FunctionBase::parameter_tuple& parameters
) {
	const std::string& executablePath(
		std::get<0>(parameters)
	);

	const std::string& stdinText(
		std::get<1>(parameters)
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

	boost::process::context context;
	context.stdout_behavior = boost::process::capture_stream();
	context.stdin_behavior  = boost::process::capture_stream();

	boost::process::child process(
		boost::process::launch(
			executablePath,
			std::vector<std::string>{""},
			context
		)
	);

	boost::process::postream& inputStream  = process.get_stdin();
	boost::process::pistream& outputStream = process.get_stdout();

	inputStream << stdinText;
	inputStream.close();

	const std::string resultText(
		(std::istreambuf_iterator<char>(outputStream)),
		(std::istreambuf_iterator<char>())
	);

	boost::process::status status = process.wait();

	if ( status.exited() ) {
		ResultNodeFacade result(domDocument, rootNode, "result");

		result.setValueNode("code",   std::to_string(status.exit_status()));
		result.setValueNode("output", resultText);
	} else {
		ResultNodeFacade result(domDocument, rootNode, "error");

		result.setValueNode("code", std::to_string(status.exit_status()));
	}

	return domDocument;
}

}
