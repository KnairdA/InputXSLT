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
	<xsl:param name="input"/>
	<xsl:param name="transformation"/>

	<xsl:copy-of select="InputXSLT:transform(
		$input,
		string($transformation)
	)"/>
</xsl:template>

<xsl:template name="implementation">
	<xsl:variable name="result">
		<xsl:call-template name="transformer">
			<xsl:with-param name="input">
				<test>
					<entries>
						<entry>Hello 1</entry>
						<entry>Hello 2</entry>
						<entry>Hello 3</entry>
						<entry>Hello 4</entry>
					</entries>
					<parameter>
						<test>21</test>
					</parameter>
				</test>
			</xsl:with-param>
			<xsl:with-param name="transformation">test.xsl</xsl:with-param>
		</xsl:call-template>
	</xsl:variable>

	<xsl:variable name="transformation" select="xalan:nodeset($result)/transformation"/>

	<xsl:choose>
		<xsl:when test="$transformation/@result = 'success'">
			<xsl:copy-of select="$transformation/test_case/transform_test/*"/>
		</xsl:when>
		<xsl:otherwise>
			<xsl:copy-of select="$transformation/*"/>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

</xsl:stylesheet>
