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
	<xsl:variable name="testContent">
		<content>
			<entry>Hello world!</entry>
		</content>
	</xsl:variable>
	<xsl:variable name="result" select="InputXSLT:write-file('test.xml', $testContent)"/>
	<xsl:variable name="actualContent" select="InputXSLT:read-file('test.xml')"/>

	<xsl:choose>
		<xsl:when test="$actualContent/self::file/@result = 'success'">
			<xsl:value-of select="$actualContent/self::file/content/entry"/>
		</xsl:when>
		<xsl:otherwise>
			Error during file io
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

</xsl:stylesheet>
