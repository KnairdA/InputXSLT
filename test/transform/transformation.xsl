<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:dyn="http://exslt.org/dynamic"
	xmlns:xalan="http://xml.apache.org/xalan"
	xmlns:InputXSLT="function.inputxslt.application"
	exclude-result-prefixes="dyn xalan InputXSLT"
>

<xsl:import href="[testcase.xsl]"/>

<xsl:template name="writer">
	<xsl:param name="file"/>
	<xsl:param name="content"/>

	<xsl:copy-of select="InputXSLT:write-file(
		$file,
		$content
	)"/>
</xsl:template>

<xsl:template name="transformer">
	<xsl:param name="input"/>
	<xsl:param name="transformation"/>
	<xsl:param name="parameters"/>

	<xsl:copy-of select="InputXSLT:transform(
		$input,
		$transformation,
		$parameters
	)"/>
</xsl:template>

<xsl:template name="generator">
	<xsl:param name="input"/>
	<xsl:param name="transformation"/>
	<xsl:param name="target"/>
	<xsl:param name="parameters"/>

	<xsl:variable name="transformerResult">
		<xsl:call-template name="transformer">
			<xsl:with-param name="input" select="string($input)"/>
			<xsl:with-param name="transformation" select="string($transformation)"/>
			<xsl:with-param name="parameters" select="xalan:nodeset($parameters)"/>
		</xsl:call-template>
	</xsl:variable>

	<xsl:variable name="writerResult">
		<xsl:call-template name="writer">
			<xsl:with-param name="file" select="string($target)"/>
			<xsl:with-param name="content" select="xalan:nodeset($transformerResult)/transformation/text()"/>
		</xsl:call-template>
	</xsl:variable>

	<generator transformation="{$transformation}" target="{$target}">
		<xsl:copy-of select="$transformerResult"/>
		<xsl:copy-of select="$writerResult"/>
	</generator>
</xsl:template>

<xsl:template name="implementation">
	<xsl:variable name="result">
		<xsl:call-template name="generator">
			<xsl:with-param name="input">[test.xml]</xsl:with-param>
			<xsl:with-param name="transformation">test.xsl</xsl:with-param>
			<xsl:with-param name="target">test_actual.xml</xsl:with-param>
			<xsl:with-param name="parameters">
				<test>21</test>
			</xsl:with-param>
		</xsl:call-template>
	</xsl:variable>

	<xsl:variable name="generator" select="xalan:nodeset($result)/generator"/>
	<xsl:variable name="transformation" select="$generator/transformation/@result"/>
	<xsl:variable name="write" select="$generator/file/@result"/>

	<xsl:choose>
		<xsl:when test="$transformation = 'success' and $write = 'success'">
			<xsl:copy-of select="
				InputXSLT:read-file('test_actual.xml')/test_case/transform_test/*
			"/>
		</xsl:when>
		<xsl:otherwise>
			<xsl:copy-of select="xalan:nodeset($result)/generator/*"/>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

</xsl:stylesheet>
