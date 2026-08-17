\ fibo.fs — числа Фибоначчи
\
\ Классический пример рекурсии. RECURSE — вызов определяемого слова
\ из его собственного тела.
\   fib(0)=fib(1)=1,  fib(n)=fib(n-1)+fib(n-2)
\
\ Запуск:  forth usr/examples/fibo.fs

: fib ( n -- fib(n) )
  dup 2 < if
    drop 1              \ n < 2  ->  1
  else
    dup 1- recurse      \ fib(n-1)
    swap 2 - recurse    \ fib(n-2)
    +                   \ fib(n-1) + fib(n-2)
  then
;

: print-fibs ( n -- )
  cr
  0 do
    i fib .
  loop
  cr
;

15 print-fibs
