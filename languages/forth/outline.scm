(source_file
  (colon) @item
  .
  (word) @name)

(source_file
  (word) @item @_definer
  .
  (word) @name
  (#any-of? @_definer "variable" "VARIABLE" "2variable" "2VARIABLE" "value" "VALUE" "create" "CREATE" "defer" "DEFER" "marker" "MARKER" "vocabulary" "VOCABULARY" "constant" "CONSTANT" "2constant" "2CONSTANT"))
