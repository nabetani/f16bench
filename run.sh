#!/bin/bash

set -eu

echo "" > result.txt
echo "clang_o3" >> result.txt
bin/clang_o3 >> result.txt
echo "clang_o2" >> result.txt
bin/clang_o2 >> result.txt

echo "clang_o3 (rosetta)" >> result.txt
bin/iclang_o3 >> result.txt
echo "clang_o2 (rosetta)" >> result.txt
bin/iclang_o2 >> result.txt

echo "gcc_o3" >> result.txt
bin/gcc_o3 >> result.txt
echo "gcc_o2" >> result.txt
bin/gcc_o2 >> result.txt
