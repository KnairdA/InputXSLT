#include "include_entity_resolver.h"

#include <xercesc/framework/LocalFileInputSource.hpp>

#include "boost/filesystem.hpp"

#include "support/xerces_string_guard.h"

namespace {

std::pair<bool, std::string> extractFilePath(const std::string& rawPath) {
	const std::size_t leadingDelimiter = rawPath.find_first_of('[');
	const std::size_t closingDelimiter = rawPath.find_last_of(']');

	if ( leadingDelimiter != std::string::npos &&
		 closingDelimiter != std::string::npos &&
		 leadingDelimiter <  closingDelimiter ) {
		return std::make_pair(
			true,
			rawPath.substr(
				leadingDelimiter + 1,
				closingDelimiter - leadingDelimiter - 1
			)
		);
	} else {
		return std::make_pair(false, std::string());
	}
}

}

namespace InputXSLT {

IncludeEntityResolver::IncludeEntityResolver(
	const std::vector<std::string>& path):
	path_() {
	this->path_.reserve(path.size());

	std::transform(
		path.begin(),
		path.end(),
		std::back_inserter(this->path_),
		[](const std::string& path) -> FilesystemContext {
			return FilesystemContext(path);
		}
	);
}

xercesc::InputSource* IncludeEntityResolver::resolveEntity(
	const XMLCh* const,
	const XMLCh* const systemId
) {
	if ( systemId != nullptr ) {
		auto filePath = extractFilePath(*XercesStringGuard<char>(systemId));

		if ( filePath.first ) {
			auto resolvedPath = this->resolve(filePath.second);

			if ( resolvedPath.first ) {
				return new xercesc::LocalFileInputSource(
					*XercesStringGuard<XMLCh>(resolvedPath.second.string())
				);
			} else {
				return nullptr;
			}
		} else {
			return nullptr;
		}
	} else {
		return nullptr;
	}
}

std::pair<bool, boost::filesystem::path> IncludeEntityResolver::resolve(
	const std::string& filePath) {
	for ( auto&& context : this->path_ ) {
		const boost::filesystem::path resolvedPath(
			context.resolve(filePath)
		);

		if ( boost::filesystem::exists(resolvedPath) &&
			 boost::filesystem::is_regular_file(resolvedPath) ) {
			return std::make_pair(true, resolvedPath);
		}
	}

	return std::make_pair(false, boost::filesystem::path());
}

}
