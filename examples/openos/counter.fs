\ counter.fs — демонстрация многозадачности на Forth
\
\ Аналог usr/bin/counter: считает от 0 до n-1, печатая «[pid P] counter: i»,
\ и периодически уступает процессор (MS — сон в миллисекундах, как sleep(300)
\ в C-шном counter).
\
\ Число шагов передаётся аргументом через --args:
\
\   forth usr/examples/counter.fs --args=100
\
\ Без аргумента — 10 шагов. Доступные слова для аргументов:
\   ARGC       ( -- n )          число аргументов программы
\   N ARG      ( n -- addr len ) N-й аргумент как строка
\   S>NUMBER   ( addr len -- n ) строка -> число

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
