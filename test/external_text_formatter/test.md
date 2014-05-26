# Markdown Test

__Lorem ipsum__ dolor sit amet, _consectetur_ adipisicing elit, sed do `eiusmod` tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.

> Duis aute irure dolor in reprehenderit in voluptate
> velit esse cillum dolore eu fugiat nulla pariatur.

Excepteur sint **occaecat** cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.

* Test 1
* Test 2
* Test 3
* Test 4

Test Test Test

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
