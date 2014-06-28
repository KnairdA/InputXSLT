<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:xalan="http://xml.apache.org/xalan"
	xmlns:InputXSLT="function.inputxslt.application"
	exclude-result-prefixes="xalan InputXSLT"
>

<xsl:output
	method="xml"
	omit-xml-declaration="no"
	encoding="UTF-8"
	indent="yes"
/>

<xsl:param name="parameters"/>

<xsl:template match="test">
<test_case>
	<transform_test>
		<input_tree>
			<xsl:copy-of select="./entry"/>
		</input_tree>
		<parameter_value>
			<xsl:copy-of select="$parameters/test * 2"/>
		</parameter_value>
	</transform_test>
</test_case>
</xsl:template>

</xsl:stylesheet>
