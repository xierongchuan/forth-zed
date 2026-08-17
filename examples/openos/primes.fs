\ primes.fs — простые числа до 100
\
\ Наивная проверка: число простое, если не делится ни на что от 2 до n-1.
\ Здесь показаны VARIABLE (переменная), ! (запись) и @ (чтение).
\
\ Запуск:  forth usr/examples/primes.fs

variable pflag          \ флаг: 1 — простое, 0 — составное

: prime? ( n -- f )
  1 pflag !             \ по умолчанию считаем простым
  dup 2 < if
    drop 0 pflag !      \ 0 и 1 — не простые
  else
    dup 2 do            \ ( n n 2 ) -> DO берёт limit=n, start=2; n остаётся
      over i mod 0= if  \ n делится на i без остатка?
        0 pflag !       \ значит составное
      then
    loop
    drop
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
