#include "read_file.h"

namespace InputXSLT {

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

	xalan::CharVectorType fileNameVector;
	std::string fileNameString;

	arguments[0]->str().transcode(fileNameVector);

	std::move(
		fileNameVector.begin(),
		fileNameVector.end(),
		fileNameString.begin()
	);

	return executionContext.getXObjectFactory().createString(
		xalan::XalanDOMString(
			InputXSLT::readFile(fileNameString).data()
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
