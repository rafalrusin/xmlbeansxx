#
#    Copyright 2004-2005 TouK s.c.
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



all-local: stamp-build 
	
stamp-build: $(shell find -regex '\./\(src\|lib\|build\.xml\).*' -not -regex '.*/\..*' | grep -v stamp)
	cd ${srcdir}; ant dist -Ddist.dir=${PWD}/dist -Dbuild.dir=${PWD}/build
	touch stamp-build

clean-local:
	cd ${srcdir}; ant clean -Ddist.dir=${PWD}/dist -Dbuild.dir=${PWD}/build
	rm -f stamp-build

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(datadir)/xmlbeansxx
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	cp -a dist/* ${DESTDIR}${datadir}/xmlbeansxx
	ln -s ${datadir}/xmlbeansxx/bin/xmlbeansxx-gen ${DESTDIR}${bindir}/xmlbeansxx-gen
	ln -s ${datadir}/xmlbeansxx/bin/scompxx ${DESTDIR}${bindir}/scompxx

uninstall-local:
	rm ${DESTDIR}${bindir}/xmlbeansxx-gen ${DESTDIR}${bindir}/scompxx
	rm -r ${DESTDIR}${datadir}/xmlbeansxx
    
dist-hook: stamp-build
	for file in `find -regex '\./\(src\|lib\|build\.xml\).*' -not -regex '.*/\..*'`; do \
		if [ -d $$file ]; then \
			mkdir $(distdir)/$$file; \
		else \
			cp -dP $$file $(distdir)/$$file; \
		fi; \
	done
