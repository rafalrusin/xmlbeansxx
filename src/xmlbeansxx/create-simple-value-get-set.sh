f() {
cat <<EOF
$1 XmlObjectBase::get${2}Value() {
    LOG4CXX_ERROR(LOG, "Not implemented");
    throw NotImplementedException();
    return $1();
}
void XmlObjectBase::set${2}Value(const ${1} &value) {
    LOG4CXX_ERROR(LOG, "Not implemented");
    throw NotImplementedException();
}
EOF
}

cat SimpleValue.txt | (while read l; do f $l; done)
