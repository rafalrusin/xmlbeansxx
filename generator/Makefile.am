#
#    Copyright 2004-2008 TouK sp. z o.o. s.k.a.
#    
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.

EXTRA_DIST = CMakeLists.txt

all-local: stamp-build 

stamp-build: $(shell find . -regex '\./\(src\|pom\.xml\).*' -not -regex '.*/\..*' | grep -v stamp)
	if [ "x`readlink -f ${srcdir}`" != "x`readlink -f .`" ]; then cp ${srcdir}/pom.xml . ; fi
	${MVN} -U -Dbase=${srcdir} -Prelease package 
	touch stamp-build

clean-local:
	${MVN} clean
	rm -f stamp-build
	if [ "x`readlink -f ${srcdir}`" != "x`readlink -f .`" ]; then rm -f pom.xml; true ; fi 

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(datadir)/xmlbeansxx
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	cp -r target/xmlbeansxx-gen/* ${DESTDIR}${datadir}/xmlbeansxx
	ln -fs ${datadir}/xmlbeansxx/bin/xmlbeansxx-gen ${DESTDIR}${bindir}/xmlbeansxx-gen
	ln -fs ${datadir}/xmlbeansxx/bin/scompxx ${DESTDIR}${bindir}/scompxx
	ln -fs ${datadir}/xmlbeansxx/bin/xmlbeansxx-split ${DESTDIR}${bindir}/xmlbeansxx-split
	ln -fs ${datadir}/xmlbeansxx/bin/xmlbeansxx-geninclude ${DESTDIR}${bindir}/xmlbeansxx-geninclude

uninstall-local:
	rm -fr ${DESTDIR}${datadir}/xmlbeansxx
	rm -f ${DESTDIR}${bindir}/xmlbeansxx-gen
	rm -f ${DESTDIR}${bindir}/scompxx
	rm -f ${DESTDIR}${bindir}/xmlbeansxx-split
	rm -f ${DESTDIR}${bindir}/xmlbeansxx-geninclude

dist-hook:
	for file in `find . -regex '\./\(src\|pom\.xml\).*' -not -regex '.*/\..*'`; do \
		if [ -d $$file ]; then \
			mkdir $(distdir)/$$file; \
		else \
			cp -dP $$file $(distdir)/$$file; \
		fi; \
	done
