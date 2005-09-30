for i in *.h; do 
    if ! grep BoostAssert <$i >/dev/null;then echo $i; 
        if ! grep include <$i >/dev/null; then
            echo WARNING $i 1>&2;
        else
            N=`grep -n include <$i | cut -f1 -d':' | head -n 1`;
            ( nr=0; while read l; do nr=$[$nr+1]; if test $N -eq $nr; then echo '#include "BoostAssert.h"'; fi; echo "$l";done) <$i >$i.2; 
        fi
    fi; 
done

