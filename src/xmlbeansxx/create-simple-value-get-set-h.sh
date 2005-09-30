f() {
cat <<EOF
virtual $1 get${2}Value();
virtual void set${2}Value(const ${1} &value);
EOF
}

cat SimpleValue.txt | (while read l; do f $l; done)
