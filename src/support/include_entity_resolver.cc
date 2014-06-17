#include "include_entity_resolver.h"

#include <xercesc/framework/LocalFileInputSource.hpp>

#include "boost/filesystem.hpp"

#include "support/xerces_string_guard.h"

namespace {

using InputXSLT::XercesStringGuard;

inline boost::filesystem::path getPathFromSystemId(
	const XMLCh* const systemId) {
	return boost::filesystem::path(
		*XercesStringGuard<char>(systemId) + 7
	);
}

boost::optional<std::string> extractFilePath(const XMLCh* const rawPath) {
	const std::string filePath         = *XercesStringGuard<char>(rawPath);
	const std::size_t leadingDelimiter = filePath.find_first_of('[');
	const std::size_t closingDelimiter = filePath.find_last_of(']');

	if ( leadingDelimiter != std::string::npos &&
		 closingDelimiter != std::string::npos &&
		 leadingDelimiter <  closingDelimiter ) {
		return boost::make_optional(
			filePath.substr(
				leadingDelimiter + 1,
				closingDelimiter - leadingDelimiter - 1
			)
		);
	} else {
		return boost::optional<std::string>();
	}
}

}

namespace InputXSLT {

IncludeEntityResolver::IncludeEntityResolver(
	const std::vector<std::string>& path):
	path_(path.begin(), path.end()) { }

xercesc::InputSource* IncludeEntityResolver::resolveEntity(
	const XMLCh* const,
	const XMLCh* const systemId
) {
	if ( systemId != nullptr ) {
		return new xercesc::LocalFileInputSource(
			*XercesStringGuard<XMLCh>(this->resolve(systemId).string())
		);
	} else {
		return nullptr;
	}
}

boost::filesystem::path IncludeEntityResolver::resolve(
	const XMLCh* const rawPath) const {
	if ( auto filePath = extractFilePath(rawPath) ) {
		if ( auto resolvedPath = this->resolve(*filePath) ) {
			return *resolvedPath;
		} else {
			return *filePath;
		}
	} else {
		return getPathFromSystemId(rawPath);
	}
}

boost::optional<boost::filesystem::path> IncludeEntityResolver::resolve(
	const boost::filesystem::path& filePath) const {
	for ( auto&& context : this->path_ ) {
		const boost::filesystem::path resolvedPath(
			context.resolve(filePath)
		);

		if ( boost::filesystem::exists(resolvedPath) &&
			 boost::filesystem::is_regular_file(resolvedPath) ) {
			return boost::make_optional(resolvedPath);
		}
	}

	return boost::optional<boost::filesystem::path>();
}

}
