#!/bin/bash
assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 "0;"
assert 42 "42;"
assert 2 "1+1;"
assert 21 "5+20-4;"
assert 41 " 12 + 34 - 5 ;"
assert 47 "5+6*7;"
assert 15 "5*(9-6);"
assert 4 "(3+5)/2;"
assert 10 "-10+20;"
assert 5 "+2+3;"
assert 1 "22 == 22;"
assert 0 "1==2;"
assert 1 "12 != 3;"
assert 0 "3 != 3;"
assert 1 "12 > 10;"
assert 0 "10 > 10;"
assert 1 "30 >= 3;"
assert 1 "2 >= 2;"
assert 0 "13 >= 15;"
assert 1 "12 < 16;"
assert 0 "5 < 2;"
assert 1 "12 <= 23;"
assert 1 "5 <= 5;"
assert 0 "9 <= 4;"
assert 3 "a = 3;"
assert 22 "b = 5 * 6 - 8;"
assert 3 "a=3; b = 5; a;"
assert 5 "a=3; b = 5; b;"
assert 2 "a=b=2;"
assert 3 "a=3; b = 1; a + b / 2;"
assert 2 "a=3; b = 1; (a + b) / 2;"

echo OK
