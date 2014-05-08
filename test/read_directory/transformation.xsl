<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:InputXSLT="function.inputxslt.application"
	exclude-result-prefixes="InputXSLT"
>

<xsl:output
	method="xml"
	omit-xml-declaration="no"
	encoding="UTF-8"
	indent="yes"
/>

<xsl:template match="/">
<test_case>
	<function_read_directory>
		<xsl:for-each select="InputXSLT:read-directory('../')[@type='directory']">
			<item><xsl:value-of select="."/></item>
		</xsl:for-each>
	</function_read_directory>
</test_case>
</xsl:template>

</xsl:stylesheet>
