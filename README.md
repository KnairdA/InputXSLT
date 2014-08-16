# InputXSLT

... is a proof-of-concept implementation of external ~~read-only~~ file access functions for XSLT based on Apache [xalan](https://xalan.apache.org/) and [xerces](https://xerces.apache.org/).

## Why?!

Contrary to popular opinion I actually like XSLT as a content transformation language and have built - amongst other things - my personal website on top of it. While I used the XSLT based [Symphony CMS](http://www.getsymphony.com/)  for that particular endeavour, the intention behind the experiment contained within this repository is to develop XSLT extensions enabling the development of static site generators using XSLT as both a template and application language.

## Current features:

- external `read-file` function for read-only access to both plain and xml files
- external `read-directory` function for read-only directory traversal
- external `transform` function for executing transformations inside transformations
- external `generate` function for executing transformations and committing the result directly to the filesystem
- external `external-command` function for executing external commands such as text formatters and capturing their output
- external `write-file` function for writing files

## Examples:

The `test` directory contains black-box test cases for every external function provided by this application which may be used as basic usage examples.

- [`InputXSLT:read-file` (plain)](test/read_file/transformation.xsl)
- [`InputXSLT:read-file` (xml)](test/read_xml_file/transformation.xsl)
- [`InputXSLT:read-directory`](test/read_directory/transformation.xsl)
- [`InputXSLT:transform`](test/transform/transformation.xsl)
- [`InputXSLT:generate`](test/generate/transformation.xsl)
- [`InputXSLT:external-command` (text formatting)](test/external_text_formatter/transformation.xsl) (requires [markdown.pl](http://daringfireball.net/projects/markdown/))
- [`InputXSLT:write-file`](test/write_file/transformation.xsl)

Concepts of how static sites may be generated using InputXSLT are being evaluated in [TestXSLT](https://github.com/KnairdA/TestXSLT).

## Requirements:

- CMake
- C++ compiler with C++11 support
- Apache [Xalan](https://xalan.apache.org/) XSLT Processor (preferably built with ICU flags)
- Apache [Xerces](https://xerces.apache.org/) XML Library
- Boost [Filesystem](http://www.boost.org/doc/libs/1_55_0/libs/filesystem/doc/index.htm)
- Boost [Program Options](http://www.boost.org/doc/libs/1_55_0/doc/html/program_options.html)
- Boost [Process](http://www.highscore.de/boost/process/index.html)
