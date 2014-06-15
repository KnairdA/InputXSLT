<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:xalan="http://xml.apache.org/xalan"
	xmlns:InputXSLT="function.inputxslt.application"
	exclude-result-prefixes="InputXSLT xalan"
>

<xsl:import href="[testcase.xsl]"/>

<xsl:template name="implementation">
	<xsl:variable name="result" select="InputXSLT:read-file('test.md')"/>

	<xsl:choose>
		<xsl:when test="$result/self::file/@result = 'success'">
			<xsl:value-of select="$result/self::file"/>
		</xsl:when>
		<xsl:otherwise>
			Error during file io
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

</xsl:stylesheet>
