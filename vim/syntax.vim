" Vim syntax file
" Language:     pkgman.conf(5) configuration file
" Maintainer:   Alexandr Savca <alexandr.savca89@gmail.com>
" Last Change:  December 31, 2024

" Quit when a syntax file was already loaded.
if exists("b:current_syntax")
  finish
endif

" We need nocompatible mode in order to continue lines with backslashes.
" Original setting will be restored.
let s:cpo_save = &cpo
set cpo&vim

" Comment.
syn keyword pkgman_todo    TODO FIXME XXX NOTE
                           \ contained

syn region  pkgman_comment start="^\s*#" end="$"
                           \ display
                           \ oneline
                           \ contains=pkgman_todo,@Spell

" Key Value.
syn region  pkgman_line    start="^[^ #]" end="$"
                           \ display
                           \ oneline
                           \ contains=pkgman_key,pkgman_comment

syn keyword pkgman_key     pkgsrcdir runscripts preferhigher useregex
                           \ readme writelog logmode rmlog_on_success
                           \ logfile makecommand addcommand
                           \ removecommand runscriptcommand
                           \ contained
                           \ nextgroup=pkgman_val
                           \ skipwhite

syn region  pkgman_val     start="\s" end="$"
                           \ display
                           \ oneline
                           \ contains=pkgman_sval,pkgman_kval

syn keyword pkgman_kval    append overwrite enabled disabled yes no
                           \ verbose compact
                           \ contained
                           \ skipwhite

syn region  pkgman_sval    start="\s.\+\s" end="$"
                           \ matchgroup=pkgman_sval
                           \ contains=pkgman_kval

" Define the default highlighting.
hi def link pkgman_todo    Todo
hi def link pkgman_comment Comment
hi def link pkgman_key     Identifier
hi def link pkgman_kval    Special
hi def link pkgman_sval    String
hi def link pkgman_val     String

let b:current_syntax = "pkgmanconf"

" Put cpoptions back the way we found it.
let &cpo = s:cpo_save
unlet s:cpo_save

" vim: cc=72
" End of file.
