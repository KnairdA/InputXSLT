#include "boost/optional.hpp"
#include "boost/program_options.hpp"
#include <boost/filesystem/fstream.hpp>

#include <string>
#include <vector>
#include <iostream>

#include "plattform_guard.h"
#include "transformation_facade.h"

class WarningGuard {
	public:
		WarningGuard(InputXSLT::TransformationFacade* transformation):
			transformation_(transformation) { };

		~WarningGuard() {
			InputXSLT::WarningCapacitor::warning_cache_ptr warnings(
				this->transformation_->getCachedWarnings()
			);

			for ( auto&& warning : *warnings ) {
				std::cerr << warning << std::endl;
			}
		};

	private:
		InputXSLT::TransformationFacade* const transformation_;

};

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
			"input",
			boost::program_options::value<std::string>(),
			"input file"
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

void handleErrors(const InputXSLT::ErrorCapacitor::error_cache& errors) {
	for ( auto&& error : errors ) {
		std::cerr << error << std::endl;
	}
}

bool process(const boost::program_options::variables_map& variables) {
	std::vector<std::string> includePath;

	if ( variables.count("include") ) {
		includePath = variables["include"].as<std::vector<std::string>>();
	};

	InputXSLT::PlattformGuard plattform(includePath);
	InputXSLT::TransformationFacade::ptr transformation{};

	if ( variables.count("input") ) {
		transformation = InputXSLT::TransformationFacade::try_create(
			handleErrors,
			variables["input"].as<std::string>().data(),
			variables["transformation"].as<std::string>().data(),
			plattform.getEntityResolver()
		);
	} else {
		transformation = InputXSLT::TransformationFacade::try_create(
			handleErrors,
			variables["transformation"].as<std::string>().data(),
			plattform.getEntityResolver()
		);
	}

	if ( transformation ) {
		WarningGuard guard(transformation.get());

		try {
			if ( variables.count("target") ) {
				boost::filesystem::ofstream file(
					variables["target"].as<std::string>()
				);

				if ( file.is_open() ) {
					transformation->generate(file);
				} else {
					return false;
				}
			} else {
				transformation->generate(std::cout);
			}

			return true;
		}
		catch (const InputXSLT::ErrorCapacitor::exception& exception) {
			handleErrors(*exception);

			return false;
		}
	}

	return false;
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
