<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:external="http://ExternalFunction.xalan-c++.xml.apache.org"
>

<xsl:output method="xml"
	doctype-public="-//W3C//DTD XHTML 1.0 Strict//EN"
	doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"
	omit-xml-declaration="yes"
	encoding="UTF-8"
	indent="yes" />

<xsl:template match="/">
<html>
	<head>
	</head>
	<body>
		<div id="raw">
			<xsl:variable name="testFile" select="external:read-file('test.txt')"/>
			<xsl:choose>
				<xsl:when test="$testFile/self::status = 'successful'">
					<xsl:value-of select="$testFile/self::content"/>
				</xsl:when>
				<xsl:otherwise>
					Could not read text file.
				</xsl:otherwise>
			</xsl:choose>
		</div>

		<div id="xml">
			<xsl:variable name="testXml" select="external:read-xml-file('test.txt')"/>
			<xsl:choose>
				<xsl:when test="$testXml/self::status = 'successful'">
					<ul id="xml">
					<xsl:for-each select="$testXml/self::content/tester/eintrag">
						<li><xsl:value-of select="."/></li>
					</xsl:for-each>
					</ul>
				</xsl:when>
				<xsl:otherwise>
					Could not read XML file.
				</xsl:otherwise>
			</xsl:choose>
		</div>

		<div id="filelists">
			<xsl:variable name="fileList" select="external:read-directory('../')"/>
			<xsl:choose>
				<xsl:when test="$fileList/self::status = 'successful'">
					<ul id="filelist1">
					<xsl:for-each select="$fileList/self::content/item[@type='file']">
						<li><xsl:value-of select="."/></li>
					</xsl:for-each>
					</ul>
					<ul id="filelist2">
					<xsl:for-each select="$fileList/self::content/item[@type='directory']">
						<li><xsl:value-of select="."/></li>
					</xsl:for-each>
					</ul>
				</xsl:when>
				<xsl:otherwise>
					Could not read directory.
				</xsl:otherwise>
			</xsl:choose>
		</div>
	</body>
</html>
</xsl:template>

</xsl:stylesheet>


