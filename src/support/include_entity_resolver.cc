#include "include_entity_resolver.h"

#include <xercesc/framework/LocalFileInputSource.hpp>

#include "boost/filesystem.hpp"

#include "support/xerces_string_guard.h"

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
		const std::string rawPath(
			*XercesStringGuard<char>(systemId)
		);

		const std::size_t leadingDelimiter = rawPath.find_first_of('[');
		const std::size_t closingDelimiter = rawPath.find_last_of(']');

		if ( leadingDelimiter != std::string::npos &&
		     closingDelimiter != std::string::npos &&
		     leadingDelimiter <  closingDelimiter ) {
			const std::string filePath(
				rawPath.substr(
					leadingDelimiter + 1,
					closingDelimiter - leadingDelimiter - 1
				)
			);

			for ( auto&& context : this->path_ ) {
				const boost::filesystem::path resolvedPath(
					context.resolve(filePath)
				);

				if ( boost::filesystem::exists(resolvedPath) &&
					 boost::filesystem::is_regular_file(resolvedPath) ) {
					return new xercesc::LocalFileInputSource(
						*XercesStringGuard<XMLCh>(resolvedPath.string())
					);
				}
			}

			return nullptr;
		} else {
			return nullptr;
		}
	} else {
		return nullptr;
	}
}

}
