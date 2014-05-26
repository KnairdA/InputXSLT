<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:dyn="http://exslt.org/dynamic"
	xmlns:xalan="http://xml.apache.org/xalan"
	xmlns:InputXSLT="function.inputxslt.application"
	exclude-result-prefixes="dyn xalan InputXSLT"
>

<xsl:include href="[testcase.xsl]"/>

<xsl:template name="formatter">
	<xsl:param name="source"/>

	<xsl:variable name="command">
		InputXSLT:external-text-formatter(
			'/usr/bin/markdown',
			$source
		)
	</xsl:variable>

	<xsl:copy-of select="dyn:evaluate($command)/output/*"/>
</xsl:template>

<xsl:template name="implementation">
	<xsl:call-template name="formatter">
		<xsl:with-param name="source" select="InputXSLT:read-file('test.md')"/>
	</xsl:call-template>
</xsl:template>

</xsl:stylesheet>
