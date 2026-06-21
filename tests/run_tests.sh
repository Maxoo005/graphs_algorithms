#!/bin/bash
set -e
BIN=/project/build/src/project
MST=/project/tests/mst_test.txt
SP=/project/tests/sp_test.txt

pass=0
fail=0

check() {
    local label="$1"
    local code="$2"
    if [ "$code" -eq 0 ]; then
        echo "[PASS] $label"
        pass=$((pass+1))
    else
        echo "[FAIL] $label (exit=$code)"
        fail=$((fail+1))
    fi
}

# --------------------------------------------------------
echo ""
echo "===== HELP ====="
$BIN --help > /dev/null 2>&1
check "help" $?

# --------------------------------------------------------
echo ""
echo "===== singleFile MST ====="

echo "-- MST lista sasiedztwa (s=2, a=1) --"
$BIN -f -p 0 -a 1 -s 2 -i $MST
check "MST lista" $?

echo "-- MST macierz incydencji (s=1, a=1) --"
$BIN -f -p 0 -a 1 -s 1 -i $MST
check "MST macierz" $?

echo "-- MST allStructures (s=0) allAlgorithms (a=0) --"
$BIN -f -p 0 -a 0 -s 0 -i $MST
check "MST allStructures/allAlg" $?

echo "-- MST startVertex=2 lista --"
$BIN -f -p 0 -a 1 -s 2 -i $MST -c 2
check "MST startVertex=2" $?

# --------------------------------------------------------
echo ""
echo "===== singleFile SP ====="

echo "-- SP lista sasiedztwa (s=2, a=3) --"
$BIN -f -p 1 -a 3 -s 2 -i $SP
check "SP lista" $?

echo "-- SP macierz incydencji (s=1, a=3) --"
$BIN -f -p 1 -a 3 -s 1 -i $SP
check "SP macierz" $?

echo "-- SP allStructures (s=0) allAlgorithms (a=0) --"
$BIN -f -p 1 -a 0 -s 0 -i $SP
check "SP allStructures/allAlg" $?

echo "-- SP startVertex=0 endVertex=3 lista --"
$BIN -f -p 1 -a 3 -s 2 -i $SP -c 0 -e 3
check "SP startVertex=0 endVertex=3" $?

echo "-- SP startVertex=1 endVertex=3 lista --"
$BIN -f -p 1 -a 3 -s 2 -i $SP -c 1 -e 3
check "SP startVertex=1 endVertex=3" $?

# --------------------------------------------------------
echo ""
echo "===== benchmark MST ====="

echo "-- benchmark MST Prim lista (s=2, a=1, l=20, d=50, n=5) --"
$BIN -b -p 0 -a 1 -s 2 -l 20 -d 50 -n 5 -r /tmp/bench_mst_list.csv
check "bench MST Prim lista" $?

echo "-- benchmark MST Prim macierz (s=1, a=1) --"
$BIN -b -p 0 -a 1 -s 1 -l 20 -d 50 -n 5 -r /tmp/bench_mst_mat.csv
check "bench MST Prim macierz" $?

echo "-- benchmark MST allAlg allStructures (a=0, s=0) --"
$BIN -b -p 0 -a 0 -s 0 -l 20 -d 50 -n 5 -r /tmp/bench_mst_all.csv
check "bench MST allAlg/allStructures" $?

# --------------------------------------------------------
echo ""
echo "===== benchmark SP ====="

echo "-- benchmark SP Dijkstra lista (s=2, a=3, l=20, d=50, n=5) --"
$BIN -b -p 1 -a 3 -s 2 -l 20 -d 50 -n 5 -r /tmp/bench_sp_list.csv
check "bench SP Dijkstra lista" $?

echo "-- benchmark SP Dijkstra macierz (s=1, a=3) --"
$BIN -b -p 1 -a 3 -s 1 -l 20 -d 50 -n 5 -r /tmp/bench_sp_mat.csv
check "bench SP Dijkstra macierz" $?

echo "-- benchmark SP allAlg allStructures (a=0, s=0) --"
$BIN -b -p 1 -a 0 -s 0 -l 20 -d 50 -n 5 -r /tmp/bench_sp_all.csv
check "bench SP allAlg/allStructures" $?

# --------------------------------------------------------
echo ""
echo "===== edge cases ====="

echo "-- brak pliku wejsciowego --"
$BIN -f -p 0 -a 1 -s 2 -i /project/tests/nonexistent.txt
check "brak pliku (powinien dzialac, nie crashowac)" $?

echo "-- benchmark gestosc 25% --"
$BIN -b -p 0 -a 1 -s 0 -l 30 -d 25 -n 3 -r /tmp/bench_d25.csv
check "bench gestosc 25" $?

echo "-- benchmark gestosc 75% --"
$BIN -b -p 0 -a 1 -s 0 -l 30 -d 75 -n 3 -r /tmp/bench_d75.csv
check "bench gestosc 75" $?

echo "-- benchmark gestosc 99% --"
$BIN -b -p 0 -a 1 -s 0 -l 30 -d 99 -n 3 -r /tmp/bench_d99.csv
check "bench gestosc 99" $?

echo "-- benchmark maly graf V=5 --"
$BIN -b -p 1 -a 3 -s 0 -l 5 -d 50 -n 3 -r /tmp/bench_v5.csv
check "bench V=5" $?

echo "-- benchmark duzy graf V=200 --"
$BIN -b -p 0 -a 1 -s 0 -l 200 -d 50 -n 3 -r /tmp/bench_v200.csv
check "bench V=200" $?

# --------------------------------------------------------
echo ""
echo "======================================="
echo "WYNIKI: PASS=$pass  FAIL=$fail"
echo "======================================="
