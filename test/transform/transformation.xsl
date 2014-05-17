<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:xalan="http://xml.apache.org/xalan"
	xmlns:InputXSLT="function.inputxslt.application"
	exclude-result-prefixes="InputXSLT xalan"
>

<xsl:output
	method="xml"
	omit-xml-declaration="no"
	encoding="UTF-8"
	indent="yes"
/>

<xsl:template name="transformer">
	<xsl:param name="transformation"/>
	<xsl:param name="target"/>
	<xsl:param name="parameters"/>

	<xsl:variable name="result" select="
		InputXSLT:transform($transformation, $target, xalan:nodeset($parameters))
	"/>
</xsl:template>

<xsl:template match="/">
<test_case>
	<transform>
		<xsl:call-template name="transformer">
			<xsl:with-param name="transformation">test.xsl</xsl:with-param>
			<xsl:with-param name="target">test_actual.xml</xsl:with-param>
			<xsl:with-param name="parameters">
				<test>21</test>
			</xsl:with-param>
		</xsl:call-template>

		<xsl:copy-of select="
			InputXSLT:read-xml-file('test_actual.xml')/test_case/transform_test
		"/>
	</transform>
</test_case>
</xsl:template>

</xsl:stylesheet>
