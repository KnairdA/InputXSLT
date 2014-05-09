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

<xsl:param name="target-file"/>
<xsl:param name="parent-directory"/>

<xsl:template match="/">
<test_case>
	<default_params>
		<target_file>
			<xsl:value-of select="$target-file"/>
		</target_file>
		<parent_directory>
			<xsl:value-of select="$parent-directory"/>
		</parent_directory>
	</default_params>
</test_case>
</xsl:template>

</xsl:stylesheet>
