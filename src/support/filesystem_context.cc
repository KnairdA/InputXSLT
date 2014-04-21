#include "filesystem_context.h"

namespace InputXSLT {

FilesystemContext::FilesystemContext(const std::string& path):
	path_(path) { }

boost::filesystem::path FilesystemContext::resolve(
	const std::string& path) const {
	return boost::filesystem::path(this->path_ / path);
}

boost::filesystem::path FilesystemContext::resolve(
	const xalan::XalanDOMString& path) const {
	xalan::CharVectorType castHelper;
	path.transcode(castHelper);

	return this->resolve(std::string(
		castHelper.begin(),
		castHelper.end()
	));
}

}
