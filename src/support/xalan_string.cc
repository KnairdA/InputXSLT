#include "xalan_string.h"

namespace InputXSLT {

std::string toString(const xalan::XalanDOMString& text) {
	xalan::CharVectorType castHelper;
	text.transcode(castHelper);

	return std::string(
		castHelper.begin(),
		castHelper.end() - 1
	);
}

xalan::XalanDOMString toString(const std::string& text) {
	return xalan::XalanDOMString(text.data());
}

}
