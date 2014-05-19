#include "resolve_include.h"

#include <algorithm>

#include "support/xalan_string.h"

namespace InputXSLT {

FunctionResolveInclude::FunctionResolveInclude(
	const std::vector<std::string>& path):
	path_(new std::vector<FilesystemContext>()) {
	this->path_->reserve(path.size());

	std::transform(
		path.begin(),
		path.end(),
		std::back_inserter(*this->path_),
		[](const std::string& path) -> FilesystemContext {
			return FilesystemContext(path);
		}
	);
}

xalan::XObjectPtr FunctionResolveInclude::execute(
	xalan::XPathExecutionContext& executionContext,
	xalan::XalanNode*,
	const xalan::XObjectPtr parameter,
	const xalan::Locator*
) const {
	const std::string filePath(toString(parameter->str()));

	for ( auto&& context : *this->path_ ) {
		const boost::filesystem::path resolvedPath(
			context.resolve(filePath)
		);

		if ( boost::filesystem::exists(resolvedPath) &&
		     boost::filesystem::is_regular_file(resolvedPath) ) {
			return executionContext.getXObjectFactory().createString(
				toString(resolvedPath.string())
			);
		}
	}

	return executionContext.getXObjectFactory().createString(
		toString("error")
	);
}

FunctionResolveInclude* FunctionResolveInclude::clone(
	xalan::MemoryManager& manager) const {
	return xalan::XalanCopyConstruct(manager, *this);
}

const xalan::XalanDOMString& FunctionResolveInclude::getError(
	xalan::XalanDOMString& result) const {
	result.assign("The function expects one parameter of type string.");

	return result;
}

}
