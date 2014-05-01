#include "plattform_guard.h"
#include "transformation_facade.h"

int main() {
	InputXSLT::PlattformGuard plattform;
	InputXSLT::TransformationFacade transformation("../dummy/transform.xsl");

	return transformation.generate("out.xml", {
		{"test", "42"}
	});
}
