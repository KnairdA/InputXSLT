#include "plattform_guard.h"
#include "transformation_facade.h"

#include "gtest/gtest.h"

int main(int argc, char **argv) {
	InputXSLT::PlattformGuard plattform;
	InputXSLT::TransformationFacade transformation("../dummy/transform.xsl");

	transformation.generate("out.xml", {
		{"test", "42"}
	});

	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
