#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanStdOutputStream.hpp>
#include <xalanc/XMLSupport/FormatterToXML.hpp>

#include "boost/optional.hpp"
#include "boost/program_options.hpp"
#include <boost/filesystem/fstream.hpp>

#include <string>
#include <vector>
#include <iostream>

#include "plattform_guard.h"
#include "transformer_facade.h"

namespace {

class WarningGuard {
	public:
		WarningGuard(InputXSLT::TransformerFacade* transformer):
			transformer_(transformer) { };

		~WarningGuard() {
			InputXSLT::WarningCapacitor::warning_cache_ptr warnings(
				this->transformer_->getCachedWarnings()
			);

			for ( auto&& warning : *warnings ) {
				std::cerr << warning << std::endl;
			}
		};

	private:
		InputXSLT::TransformerFacade* const transformer_;

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

template <typename... Arguments>
bool generate(
	InputXSLT::IncludeEntityResolver* resolver,
	std::basic_ostream<char>&         target,
	Arguments&&...                    arguments
) {
	try {
		InputXSLT::TransformerFacade transformer(resolver);
		WarningGuard guard(&transformer);

		xalan::XalanStdOutputStream         output(target);
		xalan::XalanOutputStreamPrintWriter writer(output);
		xalan::FormatterToXML               formatter(writer);

		formatter.setDoIndent(true);

		transformer.generate(
			std::forward<Arguments>(arguments)...,
			formatter
		);

		return true;
	}
	catch (const InputXSLT::ErrorCapacitor::exception& exception) {
		handleErrors(*exception);

		return false;
	}
}

bool process(const boost::program_options::variables_map& variables) {
	std::vector<std::string> includePath;

	if ( variables.count("include") ) {
		includePath = variables["include"].as<std::vector<std::string>>();
	};

	InputXSLT::PlattformGuard plattform(includePath);

	if ( variables.count("target") ) {
		boost::filesystem::ofstream file(
			variables["target"].as<std::string>()
		);

		if ( file.is_open() ) {
			if ( variables.count("input") ) {
				return generate(
					plattform.getEntityResolver(),
					file,
					variables["input"].as<std::string>().data(),
					variables["transformation"].as<std::string>().data()
				);
			} else {
				return generate(
					plattform.getEntityResolver(),
					file,
					variables["transformation"].as<std::string>().data()
				);
			}
		} else {
			return false;
		}
	} else {
		if ( variables.count("input") ) {
			return generate(
				plattform.getEntityResolver(),
				std::cout,
				variables["input"].as<std::string>().data(),
				variables["transformation"].as<std::string>().data()
			);
		} else {
			return generate(
				plattform.getEntityResolver(),
				std::cout,
				variables["transformation"].as<std::string>().data()
			);
		}
	}
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
