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

<xsl:template match="test">
<test_case>
	<transform_test>
		<input_entries>
			<xsl:copy-of select="./entries/entry"/>
		</input_entries>
		<parameter_value>
			<xsl:value-of select="./parameter/test * 2"/>
		</parameter_value>
	</transform_test>
</test_case>
</xsl:template>

</xsl:stylesheet>
