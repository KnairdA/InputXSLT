<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:InputXSLT="function.inputxslt.application"
	exclude-result-prefixes="InputXSLT"
>

<xsl:output
	method="xml"
	doctype-public="-//W3C//DTD XHTML 1.0 Strict//EN"
	doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"
	omit-xml-declaration="yes"
	encoding="UTF-8"
	indent="yes"
/>

<xsl:template match="/">
<html>
	<head>
	</head>
	<body>
		<div id="raw">
			<xsl:variable name="testFile" select="InputXSLT:read-file('test.txt')"/>
			<xsl:choose>
				<xsl:when test="$testFile/self::error">
					Could not read text file.
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$testFile[@name='test.txt']"/>
				</xsl:otherwise>
			</xsl:choose>
		</div>

		<div id="xml">
			<xsl:variable name="testXml" select="InputXSLT:read-xml-file('test.txt')"/>
			<xsl:choose>
				<xsl:when test="$testXml/self::error">
					Could not read XML file.
				</xsl:when>
				<xsl:otherwise>
					<ul id="xml">
					<xsl:for-each select="$testXml[@name='test.txt']/tester/eintrag">
						<li><xsl:value-of select="."/></li>
					</xsl:for-each>
					</ul>
				</xsl:otherwise>
			</xsl:choose>
		</div>

		<div id="filelists">
			<xsl:variable name="fileList" select="InputXSLT:read-directory('../')"/>
			<xsl:choose>
				<xsl:when test="$fileList/self::error">
					Could not read directory.
				</xsl:when>
				<xsl:otherwise>
					<ul id="filelist1">
					<xsl:for-each select="$fileList[@type='file']">
						<li><xsl:value-of select="."/></li>
					</xsl:for-each>
					</ul>
					<ul id="filelist2">
					<xsl:for-each select="$fileList[@type='directory']">
						<li><xsl:value-of select="."/></li>
					</xsl:for-each>
					</ul>
				</xsl:otherwise>
			</xsl:choose>
		</div>
	</body>
</html>
</xsl:template>

</xsl:stylesheet>


