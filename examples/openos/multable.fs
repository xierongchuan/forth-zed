\ multable.fs — таблица умножения (Пифагора) 1..9
\
\ Вложенные циклы DO...LOOP: I — индекс внешнего цикла (строка),
\ J — индекс внутреннего (столбец).
\
\ Запуск:  forth usr/examples/multable.fs

: multable
  10 1 do              \ строка I = 1..9
    10 1 do            \ столбец J = 1..9
      i j * .          \ печатаем I * J
    loop
    cr
  loop
;

multable
