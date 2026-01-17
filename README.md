# GenericHelpers

This library exists to handle a wide array of things that I've found are fairly common to things I do on a day to day basis.
I welcome any contributions to this library - I simply ask that they be written in such a way as to be as generic 
as possible, so they can be used.

All contributed code *MUST* be documented using DoxyGen typical format as this library is also created to help people
learn and understand.

Because of the doxygen configurations building this requires having texlive and pdflatex installed
(On Ubuntu texlive-latext-base texlive-latex-extra texlive-latex-recommended and pdflatex packages)

To build it is suggested using the build_with_docs.sh script that will re-generate all the documentation,
saving having to run make in docs/latex as described below.

In docs/latex there is a Makefile, and running make in docs/latex after running ninja will generate a refman.pdf
pdf reference manual.  Note that this requires pdflatex to be installed 

