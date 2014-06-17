#ifndef INPUTXSLT_SRC_SUPPORT_TUPLE_XOBJECT_VALUE_H_
#define INPUTXSLT_SRC_SUPPORT_TUPLE_XOBJECT_VALUE_H_

#include <xalanc/XPath/XObject.hpp>

#include "common.h"
#include "support/filesystem_context.h"
#include "support/include_entity_resolver.h"

namespace InputXSLT {

class XObjectValue {
	public:
		XObjectValue(
			const boost::filesystem::path&,
			const IncludeEntityResolver*
		);

		template <typename Type>
		Type get(const xalan::XObjectPtr&) const;

	private:
		const FilesystemContext filesystem_context_;
		const IncludeEntityResolver* const include_resolver_;

};

}

#endif  // INPUTXSLT_SRC_SUPPORT_TUPLE_XOBJECT_VALUE_H_
