#include "read_xml_file.h"

#include "boost/filesystem/fstream.hpp"

namespace InputXSLT {

FunctionReadXmlFile::FunctionReadXmlFile(const FilesystemContext& context):
	fs_context_(context),
	parser_() { }

FunctionReadXmlFile::FunctionReadXmlFile(const FunctionReadXmlFile& src):
	fs_context_(src.fs_context_),
	parser_() { }

xalan::XObjectPtr FunctionReadXmlFile::execute(
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

	return executionContext.getXObjectFactory().createNodeSet(
		this->parser_.parseXMLStream(
			xalan::XSLTInputSource(file)
		)
	);
}

FunctionReadXmlFile* FunctionReadXmlFile::clone(
	xalan::MemoryManager& manager) const {
	return xalan::XalanCopyConstruct(manager, *this);
}

const xalan::XalanDOMString& FunctionReadXmlFile::getError(
	xalan::XalanDOMString& result) const {
	result.assign("The read-xml-file() function expects one argument.");

	return result;
}

}
