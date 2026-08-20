\ counter.fs — пример счётчика с аргументом программы
\
\ Показывает VARIABLE, GETPID, ARGC, ARG, S>NUMBER, DO/LOOP и MS.
\ Если аргумент не передан, используется значение 10.
\
\ ARGC       ( -- n )
\ N ARG      ( n -- addr len )
\ S>NUMBER   ( addr len -- n )

variable mypid

: counter ( n -- )
  getpid mypid !
  0 do
    ." [pid " mypid @ . ." ] counter: " i . cr
    300 ms
  loop
  ." [pid " mypid @ . ." ] counter done." cr
;

: go ( -- )
  argc 0> if
    0 arg s>number
  else
    10
  then
  counter
;

go
