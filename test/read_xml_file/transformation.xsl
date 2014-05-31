<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:xalan="http://xml.apache.org/xalan"
	xmlns:InputXSLT="function.inputxslt.application"
	exclude-result-prefixes="InputXSLT xalan"
>

<xsl:include href="[testcase.xsl]"/>

<xsl:template name="implementation">
	<xsl:variable name="result">
		<xsl:copy-of select="InputXSLT:read-xml-file('test.txt')"/>
	</xsl:variable>

	<xsl:choose>
		<xsl:when test="xalan:nodeset($result)/file/@result = 'success'">
			<xsl:for-each select="xalan:nodeset($result)/file/tester/eintrag">
				<item><xsl:value-of select="."/></item>
			</xsl:for-each>
		</xsl:when>
		<xsl:otherwise>
			Error during file io
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

</xsl:stylesheet>
