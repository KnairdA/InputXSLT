#include "plattform_guard.h"
#include "transformation_facade.h"

#include "boost/program_options.hpp"

#include <string>
#include <vector>
#include <iostream>

int main(int ac, char** av) {
	boost::program_options::options_description optionDescription(
		"Supported options"
	);

	optionDescription.add_options()
		("transformation", boost::program_options::value<std::string>()->required(),  "transformation file")
		("target",         boost::program_options::value<std::string>(),              "target file")
		("include",        boost::program_options::value<std::vector<std::string>>(), "include paths")
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

	if ( variables.count("transformation") ) {
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

			return 0;
		} 
		catch (const InputXSLT::ErrorCapacitor::exception& exception) {
			for ( auto&& error : *(exception.getCachedErrors()) ) {
				std::cerr << error << std::endl;
			}

			return 1;
		}
	} else {
		std::cout << optionDescription << std::endl;

		return 1;
	}

}
