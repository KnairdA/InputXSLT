#ifndef INPUTXSLT_SRC_SUPPORT_TUPLE_MAPPER_H_
#define INPUTXSLT_SRC_SUPPORT_TUPLE_MAPPER_H_

#include <xalanc/XPath/XObject.hpp>

#include <tuple>
#include <type_traits>

#include "common.h"
#include "xobject_value.h"

namespace InputXSLT {

template <bool Condition>
using enable_if = typename std::enable_if<Condition, std::size_t>::type;

namespace Mapper {
	template <
		typename Target,
		std::size_t Index = 0,
		typename Current  = std::tuple<>,
		enable_if<Index == std::tuple_size<Target>::value> = 0
	>
	inline Target construct(
		const xalan::XPathExecutionContext::XObjectArgVectorType&,
		Current&& current
	) {
		return current;
	}

	template <
		typename Target,
		std::size_t Index = 0,
		typename Current  = std::tuple<>,
		enable_if<Index < std::tuple_size<Target>::value> = 0
	>
	inline Target construct(
		const xalan::XPathExecutionContext::XObjectArgVectorType& source,
		Current&& current = std::tuple<>()
	) {
		return construct<
			Target,
			Index + 1
		>(
			source,
			std::tuple_cat(
				current,
				std::make_tuple(
					XObjectValue::get<
						typename std::tuple_element<Index, Target>::type
					>(
						source[Index]
					)
				)
			)
		);
	}
};

}

#endif  // INPUTXSLT_SRC_SUPPORT_TUPLE_MAPPER_H_
