#include "include_entity_resolver.h"

#include <xercesc/framework/LocalFileInputSource.hpp>

#include "boost/filesystem.hpp"

#include "support/xalan_string.h"
#include "support/xerces_string_guard.h"

namespace {

boost::optional<boost::filesystem::path> extractFilePath(
	const std::string& rawPath) {
	const std::size_t leadingDelimiter = rawPath.find_first_of('[');
	const std::size_t closingDelimiter = rawPath.find_last_of(']');

	if ( leadingDelimiter != std::string::npos &&
		 closingDelimiter != std::string::npos &&
		 leadingDelimiter <  closingDelimiter ) {
		return boost::make_optional(
			boost::filesystem::path(
				rawPath.substr(
					leadingDelimiter + 1,
					closingDelimiter - leadingDelimiter - 1
				)
			)
		);
	} else {
		return boost::optional<boost::filesystem::path>();
	}
}

}

namespace InputXSLT {

boost::filesystem::path IncludeEntityResolver::getPathFromSystemId(
	const XMLCh* const systemId) {
	return boost::filesystem::path(
		*XercesStringGuard<char>(systemId) + 7
	);
}

IncludeEntityResolver::IncludeEntityResolver(
	const std::vector<std::string>& path):
	path_(path.begin(), path.end()) { }

xercesc::InputSource* IncludeEntityResolver::resolveEntity(
	const XMLCh* const,
	const XMLCh* const systemId
) {
	if ( systemId != nullptr ) {
		const std::string systemIdString(
			*XercesStringGuard<char>(systemId)
		);

		if ( auto resolvedPath = this->resolve(systemIdString) ) {
			return new xercesc::LocalFileInputSource(
				*XercesStringGuard<XMLCh>((*resolvedPath).string())
			);
		} else {
			return new xercesc::LocalFileInputSource(
				*XercesStringGuard<XMLCh>(systemIdString)
			);
		}
	} else {
		return nullptr;
	}
}

boost::optional<boost::filesystem::path> IncludeEntityResolver::resolve(
	const std::string& rawPath) const {
	if ( auto filePath = extractFilePath(rawPath) ) {
		if ( auto resolvedPath = this->tryIncludePaths(*filePath) ) {
			return boost::make_optional(*resolvedPath);
		} else {
			return boost::make_optional(*filePath);
		}
	} else {
		return boost::optional<boost::filesystem::path>();
	}
}

boost::optional<boost::filesystem::path> IncludeEntityResolver::tryIncludePaths(
	const boost::filesystem::path& filePath) const {
	for ( auto&& context : this->path_ ) {
		const boost::filesystem::path resolvedPath(
			context.resolve(filePath)
		);

		if ( boost::filesystem::exists(resolvedPath) ) {
			return boost::make_optional(resolvedPath);
		}
	}

	return boost::optional<boost::filesystem::path>();
}

}
