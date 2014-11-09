#include "filesystem_context.h"

#include <algorithm>
#include <iterator>

#include "support/xalan_string.h"

namespace {

const std::string workingDirectory(".");

boost::filesystem::path determineBasePath(
	const boost::filesystem::path& path) {
	const boost::filesystem::path basePath(
		boost::filesystem::is_directory(path) ? path : path.parent_path()
	);

	if ( basePath == boost::filesystem::current_path() ) {
		return workingDirectory;
	} else {
		if ( path.is_absolute() ) {
			return path;
		} else {
			return workingDirectory / path;
		}
	}
}

}

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
	path_(determineBasePath(path)) { }

FilesystemContext::FilesystemContext(const std::string& path):
	FilesystemContext(boost::filesystem::path(path)) { }

FilesystemContext::FilesystemContext():
	path_(workingDirectory) { }

boost::filesystem::path FilesystemContext::resolve(
	const xalan::XalanDOMString& path) const {
	return this->resolve(toString(path));
}

boost::filesystem::path FilesystemContext::resolve(
	const boost::filesystem::path& path) const {
	if ( path.is_absolute() ) {
		return path;
	} else {
		return this->path_ / path;
	}
}

boost::filesystem::path FilesystemContext::getBase() const {
	return this->path_;
}

}
