#include "read_file.h"

#include "boost/filesystem/fstream.hpp"

#include <fstream>

namespace InputXSLT {

FunctionReadFile::FunctionReadFile(const FilesystemContext& context):
	fs_context_(context) { }

xalan::XObjectPtr FunctionReadFile::execute(
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

	boost::filesystem::ifstream file(
		this->fs_context_.resolve(arguments[0]->str())
	);

	const std::string fileContent(
		(std::istreambuf_iterator<char>(file)),
		(std::istreambuf_iterator<char>())
	);

	return executionContext.getXObjectFactory().createString(
		xalan::XalanDOMString(fileContent.data())
	);
}

FunctionReadFile* FunctionReadFile::clone(
	xalan::MemoryManager& manager) const {
	return xalan::XalanCopyConstruct(manager, *this);
}

const xalan::XalanDOMString& FunctionReadFile::getError(
	xalan::XalanDOMString& result) const {
	result.assign("The read-file() function expects one argument.");

	return result;
}

}
