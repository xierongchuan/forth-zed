\ fizzbuzz.fs — классическая задача FizzBuzz
\
\ Для чисел 1..30: делится на 3 -> «Fizz», на 5 -> «Buzz»,
\ на 3 и 5 сразу -> «FizzBuzz», иначе — само число.
\ Используются MOD (остаток от деления) и вложенные IF/ELSE/THEN.
\
\ Запуск:  forth usr/examples/fizzbuzz.fs

: fizzbuzz ( n -- )
  1+ 1 do              \ I = 1..n
    i 15 mod 0= if
      ." FizzBuzz"
    else
      i 3 mod 0= if
        ." Fizz"
      else
        i 5 mod 0= if
          ." Buzz"
        else
          i .
        then
      then
    then
    space
  loop
  cr
;

30 fizzbuzz
