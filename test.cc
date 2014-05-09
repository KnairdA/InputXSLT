#include "plattform_guard.h"
#include "transformation_facade.h"

#include "boost/program_options.hpp"

#include <iostream>

int main(int ac, char** av) {
	boost::program_options::options_description optionDescription(
		"Supported options"
	);

	optionDescription.add_options()
		("transformation", boost::program_options::value<std::string>()->required(), "transformation file")
		("target",         boost::program_options::value<std::string>()->required(), "target file")
	;

	boost::program_options::variables_map variables;

	boost::program_options::store(
		boost::program_options::parse_command_line(
			ac, av, optionDescription
		),
		variables
	);

	try {
		boost::program_options::notify(variables);
	}
	catch ( std::exception& exception ) {
		std::cerr << exception.what() << std::endl;
	}

	if ( variables.count("transformation") &&
	     variables.count("target") ) {
		InputXSLT::PlattformGuard plattform;

		InputXSLT::TransformationFacade transformation(
			variables["transformation"].as<std::string>()
		);

		return transformation.generate(
			variables["target"].as<std::string>()
		);
	} else {
		std::cout << optionDescription << std::endl;

		return 1;
	}

}
