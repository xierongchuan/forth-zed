; ANS Forth / OOS Forth highlighting

(line_comment) @comment
(paren_comment) @comment

; Stack effects are documentation, but their parameter text is semantic.
; Keep the delimiters subdued, make `--` a clear separator, and make the
; input/output names read like parameters rather than comment prose.
(stack_effect
  (paren_delimiter) @comment.doc)
(stack_effect
  (paren_content) @variable.parameter)
(stack_effect
  (stack_effect_separator) @operator)

(string) @string
(number) @number
(character_literal) @constant
(colon) @keyword
(semicolon) @keyword

; In Forth, an unknown token is normally a word invocation, not a variable.
(word) @function

((word) @operator
  (#any-of? @operator "dup" "DUP" "drop" "DROP" "swap" "SWAP" "over" "OVER" "rot" "ROT" "-rot" "-ROT" "?dup" "?DUP" "nip" "NIP" "tuck" "TUCK" "pick" "PICK" "roll" "ROLL" "2dup" "2DUP" "2drop" "2DROP" "2swap" "2SWAP" "2over" "2OVER" "2rot" "2ROT" "2nip" "2NIP" "2tuck" "2TUCK" "3dup" "3DUP" "3drop" "3DROP" "3swap" "3SWAP" "depth" "DEPTH" ">r" ">R" "r>" "R>" "r@" "R@" "2>r" "2>R" "2r>" "2R>" "2r@" "2R@" "+" "-" "*" "/" "mod" "MOD" "/mod" "/MOD" "*/" "*/mod" "*/MOD" "fm/mod" "FM/MOD" "sm/rem" "SM/REM" "m*" "M*" "um*" "UM*" "um/mod" "UM/MOD" "abs" "ABS" "negate" "NEGATE" "min" "MIN" "max" "MAX" "1+" "1-" "2+" "2-" "2*" "2/" "s>d" "S>D" "d>s" "D>S" "d+" "D+" "d-" "D-" "d*" "D*" "d/" "D/" "and" "AND" "or" "OR" "xor" "XOR" "invert" "INVERT" "lshift" "LSHIFT" "rshift" "RSHIFT" "=" "<>" "<" ">" "<=" ">=" "0=" "0<>" "0<" "0>" "u<" "U<" "u>" "U>" "u<=" "U<=" "u>=" "U>=" "@" "!" "+!" "2@" "2!" "c@" "C@" "c!" "C!" "c+!" "C+!" "," "c," "C," "f," "F,"))

((word) @keyword
  (#any-of? @keyword "if" "IF" "else" "ELSE" "then" "THEN" "begin" "BEGIN" "until" "UNTIL" "again" "AGAIN" "while" "WHILE" "repeat" "REPEAT" "do" "DO" "?do" "?DO" "loop" "LOOP" "+loop" "+LOOP" "leave" "LEAVE" "?leave" "?LEAVE" "unloop" "UNLOOP" "exit" "EXIT" "recurse" "RECURSE" "case" "CASE" "of" "OF" "endof" "ENDOF" "endcase" "ENDCASE" "catch" "CATCH" "throw" "THROW" "abort" "ABORT" "bye" "BYE" "immediate" "IMMEDIATE" "postpone" "POSTPONE" "[" "]" "literal" "LITERAL" "2literal" "2LITERAL" "sliteral" "SLITERAL" "[char]" "[CHAR]" "[compile]" "[COMPILE]" "variable" "VARIABLE" "2variable" "2VARIABLE" "constant" "CONSTANT" "2constant" "2CONSTANT" "value" "VALUE" "to" "TO" "+to" "+TO" "create" "CREATE" "does>" "DOES>" "defer" "DEFER" "defer!" "DEFER!" "defer@" "DEFER@" "is" "IS" "action-of" "ACTION-OF" "marker" "MARKER" "vocabulary" "VOCABULARY" "decimal" "DECIMAL" "hex" "HEX" "binary" "BINARY" "octal" "OCTAL" ":noname" ":NONAME" "code" "CODE" ";code" ";CODE" "end-code" "END-CODE" "label" "LABEL"))

((word) @function
  (#any-of? @function "emit" "EMIT" "type" "TYPE" "cr" "CR" "space" "SPACE" "spaces" "SPACES" "." ".r" ".R" "u." "U." "u.r" "U.R" "d." "D." "d.r" "D.R" "#" "#s" "#S" "#>" "hold" "HOLD" "holds" "HOLDS" "sign" "SIGN" "<#" "h." "H." "h.r" "H.R" "key" "KEY" "key?" "KEY?" "accept" "ACCEPT" "expect" "EXPECT" "refill" "REFILL" "source" "SOURCE" "source-id" "SOURCE-ID" "parse" "PARSE" "parse-name" "PARSE-NAME" "word" "WORD" "find" "FIND" "'" "[']" "search-wordlist" "SEARCH-WORDLIST" "wordlist" "WORDLIST" "get-order" "GET-ORDER" "set-order" "SET-ORDER" "get-current" "GET-CURRENT" "set-current" "SET-CURRENT" "definitions" "DEFINITIONS" "words" "WORDS" "see" "SEE" "dump" "DUMP" ".s" ".S" "allocate" "ALLOCATE" "free" "FREE" "resize" "RESIZE" "open-file" "OPEN-FILE" "create-file" "CREATE-FILE" "close-file" "CLOSE-FILE" "delete-file" "DELETE-FILE" "rename-file" "RENAME-FILE" "file-status" "FILE-STATUS" "file-position" "FILE-POSITION" "reposition-file" "REPOSITION-FILE" "file-size" "FILE-SIZE" "resize-file" "RESIZE-FILE" "read-file" "READ-FILE" "write-file" "WRITE-FILE" "read-line" "READ-LINE" "write-line" "WRITE-LINE" "flush-file" "FLUSH-FILE" "sync-file" "SYNC-FILE" "include-file" "INCLUDE-FILE" "included" "INCLUDED" "include" "INCLUDE" "require" "REQUIRE" "required" "REQUIRED" "needs" "NEEDS" "execute" "EXECUTE" "perform" "PERFORM" "fill" "FILL" "erase" "ERASE" "move" "MOVE" "cmove" "CMOVE" "cmove>" "CMOVE>" "cells" "CELLS" "cell+" "CELL+" "chars" "CHARS" "char+" "CHAR+" "align" "ALIGN" "aligned" "ALIGNED" "allot" "ALLOT" "here" "HERE" "pad" "PAD" "tib" "TIB" "#tib" "#TIB" "page" "PAGE" "at-xy" "AT-XY" "ms" "MS" "time&date" "TIME&DATE" "utime" "UTIME" "cputime" "CPUTIME" "getpid" "GETPID" "argc" "ARGC" "arg" "ARG" "s>number" "S>NUMBER" "load-file" "LOAD-FILE" "save-file" "SAVE-FILE" "peek" "PEEK" "poke" "POKE" "call" "CALL" "hex," "HEX,"))

((word) @boolean
  (#any-of? @boolean "true" "TRUE" "false" "FALSE"))

((word) @constant.builtin
  (#any-of? @constant.builtin "bl" "BL" "base" "BASE" "state" "STATE" "data-base" "DATA-BASE"))

((word) @variable.special
  (#any-of? @variable.special "i" "I" "j" "J" ">in" ">IN"))

; Colon definition name
(source_file
  (colon)
  .
  (word) @function)

; Names created by defining words
(source_file
  (word) @_definer
  .
  (word) @variable
  (#any-of? @_definer "variable" "VARIABLE" "2variable" "2VARIABLE" "value" "VALUE" "create" "CREATE" "defer" "DEFER" "marker" "MARKER" "vocabulary" "VOCABULARY"))

(source_file
  (word) @_definer
  .
  (word) @constant
  (#any-of? @_definer "constant" "CONSTANT" "2constant" "2CONSTANT"))
