#include "read_file.h"

#include "boost/filesystem/fstream.hpp"

#include <fstream>

namespace InputXSLT {

FunctionReadFile::FunctionReadFile(const FilesystemContext& context):
	fs_context_(context) { }

xalan::XObjectPtr FunctionReadFile::execute(
	xalan::XPathExecutionContext& executionContext,
	xalan::XalanNode*,
	const xalan::XObjectPtr argument,
	const xalan::Locator*
) const {
	const boost::filesystem::path filePath(
		this->fs_context_.resolve(argument->str())
	);

	if ( boost::filesystem::is_regular_file(filePath) ) {
		boost::filesystem::ifstream file(filePath);

		const std::string fileContent(
			(std::istreambuf_iterator<char>(file)),
			(std::istreambuf_iterator<char>())
		);

		return executionContext.getXObjectFactory().createString(
			xalan::XalanDOMString(fileContent.data())
		);
	} else {
		return executionContext.getXObjectFactory().createString(
			xalan::XalanDOMString("io error")
		);
	}
}

FunctionReadFile* FunctionReadFile::clone(
	xalan::MemoryManager& manager) const {
	return xalan::XalanCopyConstruct(manager, *this);
}

const xalan::XalanDOMString& FunctionReadFile::getError(
	xalan::XalanDOMString& result) const {
	result.assign("The read-file() function expects one argument of type string.");

	return result;
}

}
