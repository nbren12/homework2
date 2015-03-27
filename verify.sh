#!/bin/sh
# This script verifies that the output of ssort is properly sorted.

echo "**********************************************************************"
echo ""
echo "This script verifies that the output was sorted properly using "
echo "sort -n. If succesful, nothing further will be printed."
echo ""
echo "**********************************************************************"
cat out.???.txt > out.txt
sort -n out.txt > out.sorted.txt
diff out.txt out.sorted.txt
