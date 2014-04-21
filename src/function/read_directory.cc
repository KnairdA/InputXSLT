#include "read_directory.h"

#include <iostream>

namespace InputXSLT {

FunctionReadDirectory::FunctionReadDirectory(const FilesystemContext& context):
	fs_context_(context) { }

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

	std::string files;

	this->fs_context_.iterate(
		arguments[0]->str(),
		[&files](const boost::filesystem::path& p) {
		files += p.string() + "\n";
	});

	return executionContext.getXObjectFactory().createString(
		xalan::XalanDOMString(files.data())
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
