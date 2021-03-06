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
	<xsl:variable name="result" select="InputXSLT:read-directory('../')"/>

	<xsl:choose>
		<xsl:when test="$result/self::directory/@result = 'success'">
			<xsl:for-each select="$result/self::directory/entry">
				<item type="{@type}">
					<name><xsl:value-of select="./name"/></name>

					<xsl:if test="@type='file' and ./name='check'">
						<extension><xsl:value-of select="./extension"/></extension>
					</xsl:if>
				</item>
			</xsl:for-each>
		</xsl:when>
		<xsl:otherwise>
			Error during directory io
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

</xsl:stylesheet>
