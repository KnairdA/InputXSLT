<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:dyn="http://exslt.org/dynamic"
	xmlns:xalan="http://xml.apache.org/xalan"
	xmlns:InputXSLT="function.inputxslt.application"
	exclude-result-prefixes="dyn xalan InputXSLT"
>

<xsl:import href="[testcase.xsl]"/>

<xsl:template name="transformer">
	<xsl:param name="transformation"/>
	<xsl:param name="target"/>
	<xsl:param name="parameters"/>

	<xsl:variable name="command">
		InputXSLT:transform(
			$transformation,
			$target,
			xalan:nodeset($parameters)
		)
	</xsl:variable>

	<xsl:copy-of select="dyn:evaluate($command)"/>
</xsl:template>

<xsl:template name="implementation">
	<xsl:variable name="result">
		<xsl:call-template name="transformer">
			<xsl:with-param name="transformation">test.xsl</xsl:with-param>
			<xsl:with-param name="target">test_actual.xml</xsl:with-param>
			<xsl:with-param name="parameters">
				<test>21</test>
			</xsl:with-param>
		</xsl:call-template>
	</xsl:variable>

	<xsl:choose>
		<xsl:when test="xalan:nodeset($result)/transformation/@result = 'success'">
			<xsl:copy-of select="
				InputXSLT:read-file('test_actual.xml')/test_case/transform_test/*
			"/>
		</xsl:when>
		<xsl:otherwise>
			<xsl:copy-of select="xalan:nodeset($result)/transformation/*"/>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

</xsl:stylesheet>
