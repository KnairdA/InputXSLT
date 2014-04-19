#include "transformer_guard.h"

int main() {
	InputXSLT::TransformerGuard guard;

	return guard.execute(
		"../dummy/transform.xsl",
		"out.xml"
	);
}
