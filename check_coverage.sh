#!/bin/bash

set -e
mkdir -p coverage_tmp
cd coverage_tmp
g++ -o main  -std=c++11 -DNDEBUG -fprofile-arcs -ftest-coverage ../unittests/test_*.cpp ../*.cpp -lgtest -lgtest_main -lpthread
./main
pwd
gcov ./test_* > tmp.log
grep -n -A2 -B1 -e "Xcode" -e "gtest" tmp.log | sed -n 's/^\([0-9]\{1,\}\).*/\1d/p' | sed -f /dev/stdin tmp.log > ../coverage.log
cd ../
rm -r coverage_tmp

echo "Coverage:"
cat coverage.log

bad_count="$(cat coverage.log | grep % | cut -d % -f 1 | awk -F: '{if($2<99)print$2}' | wc -l)"
if [[ $bad_count -eq 0 ]]; then
    (exit 0);
else
    echo "Insucfficient coverrage"
    (exit 1);
fi