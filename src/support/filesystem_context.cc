#include "filesystem_context.h"

#include "support/xerces_string_guard.h"

namespace {

inline std::string xalanToString(const xalan::XalanDOMString& text) {
	xalan::CharVectorType castHelper;
	text.transcode(castHelper);

	return std::string(
		castHelper.begin(),
		castHelper.end() - 1
	);
}

}

namespace InputXSLT {

FilesystemContext::FilesystemContext(const xalan::Locator* locator):
	path_(boost::filesystem::canonical(
		boost::filesystem::path(
			*XercesStringGuard<char>(locator->getSystemId()) + 7
		).parent_path().string()
	)) { }

boost::filesystem::path FilesystemContext::resolve(
	const std::string& path) const {
	return absolute(this->path_ / path);
}

boost::filesystem::path FilesystemContext::resolve(
	const xalan::XalanDOMString& path) const {
	return this->resolve(xalanToString(path));
}

void FilesystemContext::iterate(
	const std::string& path,
	std::function<void(const boost::filesystem::path&)> func
) const {
	const boost::filesystem::path directory(this->resolve(path));

	if ( boost::filesystem::is_directory(directory) ) {
		for ( boost::filesystem::directory_iterator iter(directory);
		      iter != boost::filesystem::directory_iterator();
		      ++iter ) {
			if ( boost::filesystem::is_regular_file(iter->status()) ||
			     boost::filesystem::is_directory(iter->status()) ) {
				func(*iter);
			}
		}
	}
}

void FilesystemContext::iterate(
	const xalan::XalanDOMString& path,
	std::function<void(const boost::filesystem::path&)> func
) const {
	this->iterate(xalanToString(path), func);
}

}
