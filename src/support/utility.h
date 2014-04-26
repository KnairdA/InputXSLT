#ifndef INPUTXSLT_SRC_SUPPORT_UTILITY_H_
#define INPUTXSLT_SRC_SUPPORT_UTILITY_H_

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

#endif  // INPUTXSLT_SRC_SUPPORT_UTILITY_H_
