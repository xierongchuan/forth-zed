\ OpenOS Forth sample

variable counter

: square ( n -- n2 )
  dup *
;

: demo
  $10001000 4 peek . cr
  s" 1305150067800000" hex, drop
  data-base + call . cr
;

10 square . cr
