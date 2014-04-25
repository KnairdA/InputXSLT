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

		return executionContext.getXObjectFactory().createNodeSet(
			this->parser_.parseXMLStream(
				xalan::XSLTInputSource(file)
			)
		);
	} else {
		return executionContext.getXObjectFactory().createString(
			xalan::XalanDOMString("io error")
		);
	}
}

FunctionReadXmlFile* FunctionReadXmlFile::clone(
	xalan::MemoryManager& manager) const {
	return xalan::XalanCopyConstruct(manager, *this);
}

const xalan::XalanDOMString& FunctionReadXmlFile::getError(
	xalan::XalanDOMString& result) const {
	result.assign("The read-xml-file() function expects one argument of type string.");

	return result;
}

}
