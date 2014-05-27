<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:InputXSLT="function.inputxslt.application"
	exclude-result-prefixes="InputXSLT"
>

<xsl:include href="[testcase.xsl]"/>

<xsl:template name="implementation">
	<xsl:for-each select="InputXSLT:read-directory('../')">
		<item type="{@type}">
			<name><xsl:value-of select="./name"/></name>

			<xsl:if test="@type='file' and ./name='check'">
				<extension><xsl:value-of select="./extension"/></extension>
			</xsl:if>
		</item>
	</xsl:for-each>
</xsl:template>

</xsl:stylesheet>
