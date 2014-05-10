#ifndef INPUTXSLT_SRC_SUPPORT_STYLESHEET_PARAMETER_GUARD_H_
#define INPUTXSLT_SRC_SUPPORT_STYLESHEET_PARAMETER_GUARD_H_

#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#include <string>
#include <unordered_map>

#include "common.h"

namespace InputXSLT {

class StylesheetParameterGuard {
	public:
		typedef std::unordered_map<std::string, std::string> map;

		explicit StylesheetParameterGuard(xalan::XalanTransformer&);
		StylesheetParameterGuard(
			xalan::XalanTransformer&,
			const map&
		);

		~StylesheetParameterGuard();

		void set(const map&);
		void set(const std::string&, const std::string&);

	private:
		xalan::XalanTransformer& transformer_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_STYLESHEET_PARAMETER_GUARD_H_
