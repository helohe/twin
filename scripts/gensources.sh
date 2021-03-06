BISON=bison
FIND=find
FLEX=flex
SED=sed
M4=m4
LN=ln

$FIND -name \*.m4h | \
while read i
do
  o="`echo \"$i\" | $SED 's,/m4/\(.*\).m4h,/\1_m4.h,g'`"
  echo "$M4 -I include < \"$i\" > \"$o\""
  $M4 -I include < "$i" > "$o"
done

for i in md5.c missing.c
do
  echo "$LN -f -n \"server/$i\"pp \"libs/libtw/$i\""
  $LN -f -n "server/$i"pp "libs/libtw/$i"
done

echo "cd server"
cd server

echo "$FLEX -o rcparse_lex.cpp rcparse.l"
$FLEX -o rcparse_lex.cpp rcparse.l

echo "$BISON --no-lines --defines -o rcparse_tab.cpp rcparse.y"
$BISON --no-lines --defines -o rcparse_tab.cpp rcparse.y

echo "cd .."
cd ..
