\ squares.fs — таблица квадратов чисел 1..10
\
\ DO ... LOOP — цикл со счётчиком I. «11 1 DO» означает I = 1..10.
\ DUP * — возвести число в квадрат (умножить само на себя).
\
\ Запуск:  forth usr/examples/squares.fs

: squares
  11 1 do
    i dup * .        \ печатаем i * i
  loop
  cr
;

squares
