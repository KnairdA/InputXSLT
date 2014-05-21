<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:InputXSLT="function.inputxslt.application"
	exclude-result-prefixes="InputXSLT"
>

<xsl:include href="[testcase.xsl]"/>

<xsl:template name="implementation">
	<xsl:for-each select="InputXSLT:read-xml-file('../common/test.txt')/tester/eintrag">
		<item><xsl:value-of select="."/></item>
	</xsl:for-each>
</xsl:template>

</xsl:stylesheet>
