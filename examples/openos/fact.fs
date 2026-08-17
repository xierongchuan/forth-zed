\ fact.fs — факториал
\
\ 5! = 5*4*3*2*1 = 120. Рекурсия через RECURSE.
\
\ Запуск:  forth usr/examples/fact.fs

: fact ( n -- n! )
  dup 1 <= if
    drop 1              \ 0! = 1! = 1
  else
    dup 1- recurse *    \ n * (n-1)!
  then
;

: print-facts ( n -- )
  cr
  1+ 1 do               \ I = 1..n
    i fact .
  loop
  cr
;

10 print-facts
