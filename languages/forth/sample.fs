\ OOS Forth sample

variable counter

: square ( n -- n2 )
  dup *
;

: arg-or-default ( -- n )
  argc 0> if
    0 arg s>number
  else
    10
  then
;

: demo ( -- )
  $10001000 4 peek . cr
  s" 1305150067800000" hex, drop
  data-base + call . cr
;

arg-or-default square . cr
