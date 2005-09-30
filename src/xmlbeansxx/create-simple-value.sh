./create-accessors.sh
cat SimpleValue.txt | (
echo 'class SimpleValue_I: '
while read l; do
j=`echo $l | sed -r 's/(.*) (.*)/\1/'`
i=`echo $l | sed -r 's/(.*) (.*)/\2/'`

echo "    public virtual ${i}Accessor_I, "
done
echo '{}'

)
