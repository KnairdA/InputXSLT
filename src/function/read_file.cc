#include "read_file.h"

#include "utility.h"

namespace InputXSLT {

FunctionReadFile::FunctionReadFile(const std::string& path):
	path_(path) { }

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

		generalError(executionContext, context, locator);
	}

	xalan::CharVectorType castHelper;
	arguments[0]->str().transcode(castHelper);

	std::string fileName(this->path_);
	fileName.reserve(fileName.size() + castHelper.size());

	std::move(
		castHelper.begin(),
		castHelper.end(),
		fileName.end()
	);

	return executionContext.getXObjectFactory().createString(
		xalan::XalanDOMString(
			InputXSLT::readFile(fileName).data()
		)
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
