#ifndef INPUTXSLT_SRC_SUPPORT_FILESYSTEM_CONTEXT_H_
#define INPUTXSLT_SRC_SUPPORT_FILESYSTEM_CONTEXT_H_

#include <xalanc/XalanDOM/XalanDOMString.hpp>

#include "boost/filesystem.hpp"

#include <string>

#include "common.h"

namespace InputXSLT {

class FilesystemContext {
	public:
		explicit FilesystemContext(const std::string&);

		boost::filesystem::path resolve(const std::string&) const;
		boost::filesystem::path resolve(const xalan::XalanDOMString&) const;

	private:
		const boost::filesystem::path path_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_FILESYSTEM_CONTEXT_H_
