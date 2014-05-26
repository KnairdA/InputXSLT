# InputXSLT

... is a proof-of-concept implementation of external read-only file access functions for XSLT based on Apache [xalan](https://xalan.apache.org/) and [xerces](https://xerces.apache.org/).

## Why?!

Contrary to popular opinion I actually like XSLT as a content transformation language and have built - amongst other things - my personal website on top of it. While I used the XSLT based [Symphony CMS](http://www.getsymphony.com/)  for that particular endeavour, the intention behind the experiment contained within this repository is to develop something like a static content management system based on XSLT.

## Current features:

- external `read-file` function for read-only access to text files
- external `read-xml-file` function for read-only access to XML files
- external `read-directory` function for read-only directory traversal
- external `transform` function for executing transformations inside transformations
- external `external-text-formatter` function for executing text formatters and capturing their XML output

## Requirements:

- CMake
- C++ compiler with C++11 support
- Apache [Xalan](https://xalan.apache.org/) XSLT Processor
- Apache [Xerces](https://xerces.apache.org/) XML Library
- Boost [Filesystem](http://www.boost.org/doc/libs/1_55_0/libs/filesystem/doc/index.htm)
- Boost [Program Options](http://www.boost.org/doc/libs/1_55_0/doc/html/program_options.html)
- Boost [Process](http://www.highscore.de/boost/process/index.html)
