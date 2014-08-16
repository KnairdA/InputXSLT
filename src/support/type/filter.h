#ifndef INPUTXSLT_SRC_SUPPORT_TYPE_FILTER_H_
#define INPUTXSLT_SRC_SUPPORT_TYPE_FILTER_H_

#include <tuple>
#include <type_traits>

namespace InputXSLT {

template <
	typename BaseReference,
	typename Head,
	typename... Tail
>
struct filter_derived {
	typedef typename std::conditional<
		std::is_base_of<BaseReference, Head>::value,
		std::tuple<Head, Tail...>,
		std::tuple<Tail...>
	>::type type;
};

}

#endif  // INPUTXSLT_SRC_SUPPORT_TYPE_FILTER_H_
