\ primes.fs — пример поиска простых чисел до 100
\
\ Показывает VARIABLE, !, @, IF/ELSE/THEN, DO/LOOP и MOD.

variable pflag
variable pnum

: prime? ( n -- f )
  1 pflag !             \ по умолчанию считаем число простым
  dup 2 < if
    drop 0 pflag !      \ 0 и 1 не являются простыми
  else
    pnum !
    pnum @ 2 do
      pnum @ i mod 0= if
        0 pflag !       \ найден делитель
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
