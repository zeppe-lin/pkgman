" Vim syntax file
" Language:     pkgman.conf(5) configuration file
" Maintainer:   Alexandr Savca <alexandr.savca89@gmail.com>
" Last Change:  January 24, 2024

" Quit when a syntax file was already loaded.
if exists("b:current_syntax")
  finish
endif

" We need nocompatible mode in order to continue lines with backslashes.
" Original setting will be restored.
let s:cpo_save = &cpo
set cpo&vim

" Comment.
syn keyword pkgmanTodo     TODO FIXME XXX NOTE
                           \ contained
syn region  pkgmanComment  start="^\s*#" end="$"
                           \ display
                           \ oneline
                           \ contains=pkgmanTodo,@Spell

" Key Value.
syn region  pkgmanLine     start="^[^ #]" end="$"
                           \ display
                           \ oneline
                           \ contains=pkgmanKey,pkgmanComment
syn keyword pkgmanKey      pkgsrcdir runscripts preferhigher useregex readme
                           \ writelog logmode rmlog_on_success logfile
                           \ makecommand addcommand removecommand
                           \ runscriptcommand
                           \ contained
                           \ nextgroup=pkgmanVal
                           \ skipwhite
syn region  pkgmanVal      start="\s" end="$"
                           \ display
                           \ oneline
                           \ contains=pkgmanSVal,pkgmanKVal
syn keyword pkgmanKVal     append overwrite enabled disabled yes no
                           \ verbose compact
                           \ contained
                           \ skipwhite
syn region  pkgmanSVal     start="\s.\+\s" end="$"
                           \ matchgroup=pkgmanSVal
                           \ contains=pkgmanKVal

" Define the default highlighting.
hi def link pkgmanTodo     Todo
hi def link pkgmanComment  Comment
hi def link pkgmanKey      Identifier
hi def link pkgmanKVal     Special
hi def link pkgmanSVal     String
hi def link pkgmanVal      String

let b:current_syntax = "pkgmanconf"

" Put cpoptions back the way we found it.
let &cpo = s:cpo_save
unlet s:cpo_save

" End of file.
