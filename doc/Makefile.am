SUBDIRS = 
EXTRA_DIST = main-page.txt Doxyfile.in

docdir  = $(DESTDIR)/$(prefix)/docs/doxygen/@PACKAGE_TARNAME@-@PACKAGE_VERSION@

.PHONY: doxygen 

all-local: doxygen

doxygen: html/api/index.html
html/api/index.html: Doxyfile main-page.txt
	mkdir -p html
	@DOXYGEN@
