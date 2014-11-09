#ifndef INPUTXSLT_SRC_SUPPORT_XERCES_STRING_GUARD_H_
#define INPUTXSLT_SRC_SUPPORT_XERCES_STRING_GUARD_H_

#include <xercesc/util/XMLString.hpp>

#include <string>

namespace InputXSLT {

template <typename Type>
class XercesStringGuard {
	public:
		XercesStringGuard(const Type* src):
			string_(src) { }

		XercesStringGuard(
			const std::basic_string<
				typename std::remove_pointer<decltype(
					xercesc::XMLString::transcode(new Type[1])
				)>::type
			>& src
		):
			string_(xercesc::XMLString::transcode(src.data())) { }

		~XercesStringGuard() {
			xercesc::XMLString::release(
				const_cast<Type**>(&this->string_)
			);
		}

		const Type* operator*() const {
			return this->string_;
		}

	private:
		const Type* const string_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_XERCES_STRING_GUARD_H_
