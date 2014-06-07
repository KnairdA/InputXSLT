#ifndef INPUTXSLT_SRC_SUPPORT_TYPE_SEQUENCE_H_
#define INPUTXSLT_SRC_SUPPORT_TYPE_SEQUENCE_H_

#include <cstddef>
#include <type_traits>

namespace InputXSLT {

template <std::size_t...>
struct Sequence {
	typedef Sequence type;
};

template <
	std::size_t Size,
	std::size_t Index = 0,
	std::size_t... Current
>
struct IndexSequence {
	typedef typename std::conditional<
		Index < Size,
		IndexSequence<Size, Index + 1, Current..., Index>,
		Sequence<Current...>
	>::type::type type;
};

}

#endif  // INPUTXSLT_SRC_SUPPORT_TYPE_SEQUENCE_H_
