#include "plattform_guard.h"
#include "transformer_facade.h"

int main() {
	InputXSLT::PlattformGuard plattform;
	InputXSLT::TransformerFacade transformer("../dummy/");

	return transformer.execute(
		"../dummy/transform.xsl",
		"out.xml"
	);
}
