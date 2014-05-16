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

<xsl:template match="/">
<test_case>
	<transform>
		<xsl:variable name="argument">
			<key>test</key>
			<value>42</value>
		</xsl:variable>

		<xsl:variable
			name   = "result"
			select = "InputXSLT:transform('test.xsl', 'test_actual.xml', $argument)"
		/>

		<xsl:copy-of select="
			InputXSLT:read-xml-file('test_actual.xml')/test_case/transform_test
		"/>
	</transform>
</test_case>
</xsl:template>

</xsl:stylesheet>
