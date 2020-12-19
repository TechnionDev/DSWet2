#!/bin/bash

set -e
mkdir -p coverage_tmp
cd coverage_tmp
g++ -o main  -std=c++11 -DNDEBUG -DTEST_COVERAGE -fprofile-arcs -ftest-coverage ../unittests/test_*.cpp ../*.cpp -lgtest -lgtest_main -lpthread
./main
pwd
lcov --capture --directory ./ --output-file coverage.info | grep -v Processing
genhtml coverage.info --output-directory ../coverage_html | grep -v Processing
gcovr -r ../ > ../coverage.log
#grep -n -A2 -B1 -e "Xcode" -e "gtest" -e "/user/include" tmp.log | sed -n 's/^\([0-9]\{1,\}\).*/\1d/p' | sed -f /dev/stdin tmp.log > ../coverage.log
cd ../
rm -r coverage_tmp

echo "Coverage:"
cat coverage.log

bad_count="$(cat coverage.log | tr -s ' ' | cut -d " " -f 4 | grep % | cut -d % -f 1 | awk -F: '{if($2<98.5)print$2}' | wc -l)"
if [[ $bad_count -eq 0 ]]; then
    (exit 0);
else
    echo "Insucfficient coverrage"
    (exit 0);
fi