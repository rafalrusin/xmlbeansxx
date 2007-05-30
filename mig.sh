#xmlbeansxx migration script
if [ $# -eq 0 ]; then cat <<EOF
	Converts C++ source to newer xmlbeansxx version.
	Usage:
	$0 file_name
EOF
exit 1
fi
cp $1 $1.bk
cat $1.bk \
| sed "s/\[xmlbeans++\/xmlbeans.h\]/\[xmlbeansxx\/xmlbeansxx.h\]/g" \
| sed "s/::create/::Factory::newInstance/g" \
| sed "s/::createArray/::Factory::createArrayFn/g" \
| sed "s/::parse/::Factory::parse/g" \
| sed "s/::fromString/::Factory::parse/g" \
| sed "s/_array/Array/g" \
| sed "s/_attr/Attr/g" \
| sed "s/cgetAt/cgetArray/g" \
| sed "s/getAt/getArray/g" \
| sed "s/setAt/setArray/g" \
| sed "s/org::apache::xmlbeans/xmlbeansxx/g" \
| sed "s/#include <xmlbeans++\/xmlbeans.h>/#include <xmlbeansxx\/xmlbeansxx.h>/g" \
| sed "s/xmlbeans++/xmlbeansxx/g" \
| sed "s/Xmlbeans++/xmlbeansxx/g" \
| sed "s/xmlbeanspp/xmlbeansxx/g" \
| sed "s/pl\.touk\.xmlbeansxxgen/pl\.touk\.xmlbeansxx/g" \
| sed "s/sharedArray/shared_array/g" \
| cat - >$1

