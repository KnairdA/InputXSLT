#include "read_directory.h"

#include <sstream>

namespace InputXSLT {

FunctionReadDirectory::FunctionReadDirectory(const FilesystemContext& context):
	fs_context_(context),
	parser_() { }

FunctionReadDirectory::FunctionReadDirectory(const FunctionReadDirectory& src):
	fs_context_(src.fs_context_),
	parser_() { }

xalan::XObjectPtr FunctionReadDirectory::execute(
	xalan::XPathExecutionContext&                executionContext,
	xalan::XalanNode*                            context,
	const xalan::Function::XObjectArgVectorType& arguments,
	const xalan::Locator*                        locator
) const {
	if ( arguments.size() != 1 ) {
		xalan::XPathExecutionContext::GetAndReleaseCachedString guard(
			executionContext
		);

		this->generalError(executionContext, context, locator);
	}

	std::stringstream stream;
	stream << "<content>";

	this->fs_context_.iterate(
		arguments[0]->str(),
		[&stream](const boost::filesystem::path& p) {
		stream << "<file>" << p.filename().string() << "</file>";
	});

	stream << "</content>";

	return executionContext.getXObjectFactory().createNodeSet(
		this->parser_.parseXMLStream(
			xalan::XSLTInputSource(stream)
		)
	);
}

FunctionReadDirectory* FunctionReadDirectory::clone(
	xalan::MemoryManager& manager) const {
	return xalan::XalanCopyConstruct(manager, *this);
}

const xalan::XalanDOMString& FunctionReadDirectory::getError(
	xalan::XalanDOMString& result) const {
	result.assign("The read-directory() function expects one argument.");

	return result;
}

}
