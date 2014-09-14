# InputXSLT

…is a Apache [Xalan](https://xalan.apache.org/) and [Xerces](https://xerces.apache.org/) based XSLT extension enabling access to external commands, the filesystem and calling further transformations from inside a transformation.

## Why?!

Contrary to popular opinion I actually like XSLT as a content transformation language and have built - amongst other things - my personal website on top of it. While I used the XSLT based [Symphony CMS](http://www.getsymphony.com/)  for that particular endeavour, the intention behind this project is to develop XSLT extensions enabling the development of static site generators using XSLT as both a template and application language.

## Current features:

- external `read-file` function for read-only access to both plain and xml files
- external `read-directory` function for read-only directory traversal
- external `generate` function for executing transformations and optionally committing the result directly to the filesystem
- external `external-command` function for executing external commands such as text formatters and capturing their output
- external `write-file` function for writing files

## Examples:

The `test` directory contains black-box test cases for every external function provided by this application which may be used as basic usage examples.

- [`InputXSLT:read-file` (plain)](test/read_file/transformation.xsl)
- [`InputXSLT:read-file` (xml)](test/read_xml_file/transformation.xsl)
- [`InputXSLT:read-directory`](test/read_directory/transformation.xsl)
- [`InputXSLT:generate`](test/transform/transformation.xsl)
- [`InputXSLT:generate` (fs)](test/generate/transformation.xsl)
- [`InputXSLT:external-command` (text formatting)](test/external_text_formatter/transformation.xsl) (requires [markdown.pl](http://daringfireball.net/projects/markdown/))
- [`InputXSLT:write-file`](test/write_file/transformation.xsl)

The [new version of my personal blog](https://github.com/KnairdA/blog.kummerlaender.eu) is currently being developed based on this project and makes for a good example of how static sites may be generated using InputXSLT.

## Requirements:

- CMake
- C++ compiler with C++14 support
- Apache [Xalan](https://xalan.apache.org/) XSLT Processor (preferably built with ICU flags)
- Apache [Xerces](https://xerces.apache.org/) XML Library
- Boost [Filesystem](http://www.boost.org/doc/libs/1_55_0/libs/filesystem/doc/index.htm)
- Boost [Program Options](http://www.boost.org/doc/libs/1_55_0/doc/html/program_options.html)
- Boost [Process](http://www.highscore.de/boost/process/index.html)
