cat SimpleValue.txt | (
while read l; do
j=`echo $l | sed -r 's/(.*) (.*)/\1/'`
i=`echo $l | sed -r 's/(.*) (.*)/\2/'`

cat <<EOF
class ${i}Accessor_I {
    public:
    virtual ~${i}Accessor_I() {}
    virtual $j get${i}Value() = 0;
    virtual void set${i}Value(const $j &value) = 0;
};
EOF
done
)
