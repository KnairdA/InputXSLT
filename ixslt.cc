#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanStdOutputStream.hpp>
#include <xalanc/XMLSupport/FormatterToXML.hpp>

#include <boost/optional.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem/fstream.hpp>

#include <string>
#include <vector>
#include <iostream>

#include "platform_guard.h"
#include "transformer_facade.h"
#include "support/xerces_string_guard.h"

namespace {

class WarningGuard {
	public:
		WarningGuard(InputXSLT::TransformerFacade* transformer):
			transformer_(transformer) { }

		~WarningGuard() {
			InputXSLT::WarningCapacitor::warning_cache_ptr warnings(
				this->transformer_->getCachedWarnings()
			);

			for ( auto&& warning : *warnings ) {
				std::cerr << warning << std::endl;
			}
		}

	private:
		InputXSLT::TransformerFacade* const transformer_;

};

class StreamInputSource {
	public:
		StreamInputSource(
			const boost::filesystem::path& actualPath,
			const boost::filesystem::path& contextPath
		):
			StreamInputSource(
				std::make_unique<boost::filesystem::ifstream>(
					actualPath
				),
				contextPath
			) { }

		StreamInputSource(
			std::unique_ptr<std::istream>&& inputStream,
			const boost::filesystem::path&  contextPath
		):
			stream_(std::move(inputStream)),
			source_(stream_.get()) {
				this->source_.setSystemId(
					*InputXSLT::XercesStringGuard<XMLCh>(
						"file://" + contextPath.string()
					)
				);
			}

		xalan::XSLTInputSource& operator*() {
			return this->source_;
		}

	private:
		std::unique_ptr<std::istream> stream_;
		xalan::XSLTInputSource        source_;

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

	InputXSLT::PlatformGuard platform(includePath);

	StreamInputSource transformationSource(
		variables["transformation"].as<std::string>(),
		boost::filesystem::current_path()
	);

	if ( variables.count("target") ) {
		boost::filesystem::ofstream file(
			variables["target"].as<std::string>()
		);

		if ( file.is_open() ) {
			if ( variables.count("input") ) {
				return generate(
					platform.getEntityResolver(),
					file,
					variables["input"].as<std::string>().data(),
					*transformationSource
				);
			} else {
				return generate(
					platform.getEntityResolver(),
					file,
					*transformationSource
				);
			}
		} else {
			return false;
		}
	} else {
		if ( variables.count("input") ) {
			return generate(
				platform.getEntityResolver(),
				std::cout,
				variables["input"].as<std::string>().data(),
				*transformationSource
			);
		} else {
			return generate(
				platform.getEntityResolver(),
				std::cout,
				*transformationSource
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
