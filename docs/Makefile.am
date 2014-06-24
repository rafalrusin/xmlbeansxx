SUBDIRS =
EXTRA_DIST = Doxyfile.in

docdir  = $(DESTDIR)/$(prefix)/doc/@PACKAGE_TARNAME@-@PACKAGE_VERSION@

docs: html/index.html

html/index.html: Doxyfile
	@DOXYGEN@

install-data-local:
	$(mkinstalldirs) $(docdir)
	cp -r html/. $(docdir) ; true

uninstall-local:
	$(RM) -r -f $(docdir)

clean-local:
	$(RM) -fr html latex
