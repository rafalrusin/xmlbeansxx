


#stamp-build: $(shell find . -regex '\./\(src\|pom\.xml\).*' -not -regex '.*/\..*' | grep -v stamp)
#	cp ${srcdir}/pom.xml .; mvn -Dbase=${srcdir} -Prelease package 
#	touch stamp-build


ADD_CUSTOM_TARGET( generator
	mvn -Dbase=. -Prelease package 
	)
	