echo "not input"
cmd=" ./a.out"
echo $cmd
eval $cmd


echo "test invalid base"
cmd=" ./a.out 10  b 11 c"
echo $cmd
eval $cmd

echo "base out of range 2-200"
cmd=" ./a.out 10 2 10 201 "
echo $cmd
eval $cmd

echo "test wrong number"
cmd=" ./a.out a 2 b 23 "
echo $cmd
eval $cmd

echo "valid numbers but  digits not in 0~(base-1)"
cmd=" ./a.out  7 7 6 6 "
echo $cmd
eval $cmd

echo "decimal number add integral number"
cmd=" ./a.out  3.3 10 6 9 "
echo $cmd
eval $cmd

echo "a non-terminating/repeating number adds a integral";
cmd=" ./a.out  0.1 3  4 5"
echo $cmd
eval $cmd

echo "two extermly large numbers add each other"
cmd=" ./a.out  12345678.987654321 200 327131273612893123891293812983912.233284329483243892239 199"
echo $cmd
eval $cmd

