#include "boost/optional.hpp"
#include "boost/program_options.hpp"

#include <string>
#include <vector>
#include <iostream>

#include "plattform_guard.h"
#include "transformation_facade.h"
#include "support/error/error_capacitor.h"

boost::optional<boost::program_options::variables_map> input(
	int argc,
	char** argv
) {
	boost::program_options::options_description optionDescription(
		"Supported options"
	);

	optionDescription.add_options()
		(
			"transformation",
			boost::program_options::value<std::string>()->required(),
			"transformation file"
		)
		(
			"target",
			boost::program_options::value<std::string>(),
			"target file"
		)
		(
			"include",
			boost::program_options::value<std::vector<std::string>>(),
			"include paths"
		)
	;

	boost::program_options::variables_map variables;

	try {
		boost::program_options::store(
			boost::program_options::parse_command_line(
				argc, argv, optionDescription
			),
			variables
		);

		boost::program_options::notify(variables);
	}
	catch ( const std::exception& exception ) {
		std::cerr << exception.what()  << std::endl;
		std::cout << optionDescription << std::endl;

		return boost::optional<boost::program_options::variables_map>();
	}

	return boost::make_optional(variables);
}

bool process(const boost::program_options::variables_map& variables) {
	std::vector<std::string> includePath;

	if ( variables.count("include") ) {
		includePath = variables["include"].as<std::vector<std::string>>();
	};

	InputXSLT::PlattformGuard plattform(includePath);

	try {
		InputXSLT::TransformationFacade transformation(
			variables["transformation"].as<std::string>(),
			plattform.getEntityResolver()
		);

		if ( variables.count("target") ) {
			transformation.generate(
				variables["target"].as<std::string>()
			);
		} else {
			transformation.generate(std::cout);
		}

		return true;
	}
	catch (const InputXSLT::ErrorCapacitor::exception& exception) {
		for ( auto&& error : *(exception.getCachedErrors()) ) {
			std::cerr << error << std::endl;
		}

		return false;
	}
}

int main(int argc, char** argv) {
	if ( auto variables = input(argc, argv) ) {
		if ( process(*variables) ) {
			return 0;
		} else {
			return 1;
		}
	} else {
		return 1;
	}
}
