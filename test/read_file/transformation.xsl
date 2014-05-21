<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:InputXSLT="function.inputxslt.application"
	exclude-result-prefixes="InputXSLT"
>

<xsl:include href="[testcase.xsl]"/>

<xsl:template name="implementation">
	<xsl:value-of select="InputXSLT:read-file('../common/test.txt')"/>
</xsl:template>

</xsl:stylesheet>
