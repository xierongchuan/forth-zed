\ primes.fs — простые числа до 100
\
\ Наивная проверка: число простое, если не делится ни на что от 2 до n-1.
\ Здесь показаны VARIABLE (переменная), ! (запись) и @ (чтение).
\
\ Запуск:  forth usr/examples/primes.fs

variable pflag
variable pnum

: prime? ( n -- f )
  1 pflag !
  dup 2 < if
    drop 0 pflag !
  else
    pnum !
    pnum @ 2 do
      pnum @ i mod 0= if
        0 pflag !
      then
    loop
  then
  pflag @
;

: print-primes ( n -- )
  2 do
    i prime? if
      i .
    then
  loop
  cr
;

100 print-primes
