#include "transformation_facade.h"

#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XalanTransformer/XalanCompiledStylesheet.hpp>

#include <sstream>
#include <iostream>

namespace InputXSLT {

TransformationFacade::TransformationFacade(const std::string& transformation):
	transformation_{},
	transformer_() {
	this->transformer_.compileStylesheet(
		xalan::XSLTInputSource(transformation.data()),
		this->transformation_
	);
}

TransformationFacade::~TransformationFacade() {
	this->transformer_.destroyStylesheet(
		this->transformation_
	);
}

int TransformationFacade::generate(const std::string& target) {
	std::stringstream       emptyStream("<dummy/>");
	xalan::XSLTInputSource  inputSource(emptyStream);
	xalan::XSLTResultTarget outputTarget(target.data());


	const int resultCode = this->transformer_.transform(
		inputSource,
		this->transformation_,
		outputTarget
	);

	if ( resultCode != 0 ) {
		std::cerr << this->transformer_.getLastError() << std::endl;
	}

	return resultCode;
}

int TransformationFacade::generate(
	const std::string& target,
	const parameter_map& parameters
) {
	for ( auto&& parameter : parameters ) {
		this->transformer_.setStylesheetParam(
			parameter.first.data(),
			parameter.second.data()
		);
	}

	const int resultCode = this->generate(target);

	this->transformer_.clearStylesheetParams();

	return resultCode;
}

}
