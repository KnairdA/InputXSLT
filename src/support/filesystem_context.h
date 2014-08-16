#ifndef INPUTXSLT_SRC_SUPPORT_FILESYSTEM_CONTEXT_H_
#define INPUTXSLT_SRC_SUPPORT_FILESYSTEM_CONTEXT_H_

#include <xalanc/XalanDOM/XalanDOMString.hpp>
#include <xalanc/XPath/Function.hpp>

#include <boost/filesystem.hpp>

#include <string>
#include <functional>

#include "common.h"

namespace InputXSLT {

class FilesystemContext {
	public:
		static void iterate(
			const boost::filesystem::path&,
			const std::function<void(const boost::filesystem::path&)>&
		);

		explicit FilesystemContext(const boost::filesystem::path&);
		explicit FilesystemContext(const std::string&);

		boost::filesystem::path resolve(const xalan::XalanDOMString&) const;
		boost::filesystem::path resolve(const boost::filesystem::path&) const;

		boost::filesystem::path getBase() const;

	private:
		const boost::filesystem::path path_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_FILESYSTEM_CONTEXT_H_
