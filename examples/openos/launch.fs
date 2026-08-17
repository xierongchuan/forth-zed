\ launch.fs — обратный отсчёт (запуск ракеты)
\
\ Показывает цикл BEGIN ... UNTIL и слова DUP, 1-, 0=, DROP.
\ UNTIL повторяет тело, пока флаг на стеке равен 0 (ложь).
\
\ Запуск:  forth usr/examples/launch.fs

: launch ( n -- )
  begin
    dup .            \ печатаем текущее число
    1-               \ n = n - 1
    dup 0=           \ n - 1 == 0 ?
  until              \ если ложь — повторяем
  drop               \ убираем оставшийся 0
  ." Поехали!" cr
;

10 launch
