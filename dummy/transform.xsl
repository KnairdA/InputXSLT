<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:external="http://ExternalFunction.xalan-c++.xml.apache.org">

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
		<xsl:apply-templates select="items/item" />
	</body>
</html>
</xsl:template>

<xsl:template match="items/item">
	<div id="{.}">
		<xsl:value-of select="external:read-file('dummy/test.txt')" />
	</div>
</xsl:template>

</xsl:stylesheet>


