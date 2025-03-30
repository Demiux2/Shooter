augroup filetypedetect
au! BufRead,BufNewFile *.swnm set filetype=Shooter Without Name Map
augroup END

syntax match SWNMkeyword /\[\(WALLS\|SECTORS\)\]/
syntax match SWNMnumber /\v[+-]?\d+(\.\d+)?/

highlight SWNMkeyword ctermfg=yellow guifg=yellow
highlight SWNMnumber ctermfg=cyan guifg=cyan
