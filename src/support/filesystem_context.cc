#include "filesystem_context.h"

#include <algorithm>
#include <iterator>

#include "support/xalan_string.h"
#include "support/xerces_string_guard.h"

namespace InputXSLT {

FilesystemContext::FilesystemContext(const xalan::Locator* locator):
	path_(boost::filesystem::canonical(
		boost::filesystem::path(
			*XercesStringGuard<char>(locator->getSystemId()) + 7
		).parent_path().string()
	)) { }

FilesystemContext::FilesystemContext(const std::string& path):
	path_(boost::filesystem::canonical(path)) { }

boost::filesystem::path FilesystemContext::resolve(
	const std::string& path) const {
	return absolute(this->path_ / path);
}

boost::filesystem::path FilesystemContext::resolve(
	const xalan::XalanDOMString& path) const {
	return this->resolve(toString(path));
}

void FilesystemContext::iterate(
	const boost::filesystem::path& directory,
	std::function<void(const boost::filesystem::path&)> func
) const {
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

	for ( auto&& item : directoryItems ) {
		func(item);
	}
}

void FilesystemContext::iterate(
	const std::string& path,
	std::function<void(const boost::filesystem::path&)> func
) const {
	this->iterate(this->resolve(path), func);
}

void FilesystemContext::iterate(
	const xalan::XalanDOMString& path,
	std::function<void(const boost::filesystem::path&)> func
) const {
	this->iterate(toString(path), func);
}

}
