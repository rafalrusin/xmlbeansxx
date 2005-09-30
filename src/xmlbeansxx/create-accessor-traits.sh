cat SimpleValue.txt | (
while read l; do
j=`echo $l | sed -r 's/(.*) (.*)/\1/'`
i=`echo $l | sed -r 's/(.*) (.*)/\2/'`

cat <<EOF
template <typename T>
struct AccessorTraits<T, $j> {
    static void set(const T& obj, const $j &value) {
        obj->set${i}Value(value);
    }
    static $j get(const T& obj) {
        return obj->get${i}Value();
    }
};
EOF
done
)
