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

<xsl:template name="formatter">
	<xsl:param name="format"/>
	<xsl:param name="file"/>

	<xsl:variable name="content" select="InputXSLT:read-file($file)/text()"/>

	<xsl:copy-of select="InputXSLT:external-text-formatter(
		$format,
		$content
	)"/>
</xsl:template>

<xsl:template name="implementation">
	<xsl:variable name="result">
		<xsl:call-template name="formatter">
			<xsl:with-param name="format">markdown</xsl:with-param>
			<xsl:with-param name="file">[test.md]</xsl:with-param>
		</xsl:call-template>
	</xsl:variable>

	<xsl:choose>
		<xsl:when test="xalan:nodeset($result)/output/@result = 'success'">
			<xsl:copy-of select="xalan:nodeset($result)/output/*"/>
		</xsl:when>
		<xsl:otherwise>
			Failure during external text formatting
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

</xsl:stylesheet>
