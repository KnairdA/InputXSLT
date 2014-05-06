#ifndef INPUTXSLT_SRC_SUPPORT_FILESYSTEM_CONTEXT_H_
#define INPUTXSLT_SRC_SUPPORT_FILESYSTEM_CONTEXT_H_

#include <xalanc/XalanDOM/XalanDOMString.hpp>
#include <xalanc/XPath/Function.hpp>

#include "boost/filesystem.hpp"

#include <string>
#include <functional>

#include "common.h"

namespace InputXSLT {

class FilesystemContext {
	public:
		explicit FilesystemContext(const xalan::Locator*);
		explicit FilesystemContext(const std::string&);

		boost::filesystem::path resolve(const std::string&) const;
		boost::filesystem::path resolve(const xalan::XalanDOMString&) const;

		void iterate(const boost::filesystem::path&,
		             std::function<void(const boost::filesystem::path&)>) const;
		void iterate(const std::string&,
		             std::function<void(const boost::filesystem::path&)>) const;
		void iterate(const xalan::XalanDOMString&,
		             std::function<void(const boost::filesystem::path&)>) const;

	private:
		const boost::filesystem::path path_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_FILESYSTEM_CONTEXT_H_
