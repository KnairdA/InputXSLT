#include "filesystem_context.h"

#include <algorithm>
#include <iterator>

#include "support/xalan_string.h"

namespace InputXSLT {

void FilesystemContext::iterate(
	const boost::filesystem::path& directory,
	const std::function<void(const boost::filesystem::path&)>& func
) {
	std::vector<boost::filesystem::path> directoryItems;

	std::copy_if(
		boost::filesystem::directory_iterator(directory),
		boost::filesystem::directory_iterator(),
		std::back_inserter(directoryItems),
		[](const boost::filesystem::path& p) -> bool {
			return boost::filesystem::is_regular_file(p) ||
			       boost::filesystem::is_directory(p);
		}
	);

	std::sort(
		directoryItems.begin(),
		directoryItems.end()
	);

	std::for_each(
		directoryItems.begin(),
		directoryItems.end(),
		func
	);
}

FilesystemContext::FilesystemContext(const boost::filesystem::path& path):
	path_(boost::filesystem::canonical(
		path.parent_path()
	)) { }

FilesystemContext::FilesystemContext(const std::string& path):
	path_(boost::filesystem::canonical(path)) { }

boost::filesystem::path FilesystemContext::resolve(
	const xalan::XalanDOMString& path) const {
	return this->resolve(toString(path));
}

boost::filesystem::path FilesystemContext::resolve(
	const boost::filesystem::path& path) const {
	if ( path.is_absolute() ) {
		return path;
	} else {
		return absolute(this->path_ / path);
	}
}

}
