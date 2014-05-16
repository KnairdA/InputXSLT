<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:xalan="http://xml.apache.org/xalan"
	xmlns:InputXSLT="function.inputxslt.application"
	exclude-result-prefixes="InputXSLT xalan"
>

<xsl:output
	method="xml"
	omit-xml-declaration="no"
	encoding="UTF-8"
	indent="yes"
/>

<xsl:param name="arguments"/>

<xsl:template match="/">
<test_case>
	<transform_test>
		<argument_value>
			<xsl:value-of select="xalan:nodeset($arguments)/value"/>
		</argument_value>
		<transformation_content>
			<xsl:value-of select="InputXSLT:read-file('transformation.xsl')"/>
		</transformation_content>
	</transform_test>
</test_case>
</xsl:template>

</xsl:stylesheet>
