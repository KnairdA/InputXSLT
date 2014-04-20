#include "read_file.h"

#include "boost/filesystem/fstream.hpp"

#include <fstream>

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

	const std::string fileName(
		castHelper.begin(),
		castHelper.end()
	);

	boost::filesystem::ifstream file(this->path_ / fileName);

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
