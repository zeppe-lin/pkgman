" Vim syntax file
" Language:     pkgman.conf(5) configuration file
" Maintainer:   Alexandr Savca <alexandr.savca89@gmail.com>
" Last Change:  January 18, 2024

" quit when a syntax file was already loaded.
if exists("b:current_syntax")
  finish
endif

let s:cpo_save = &cpo
set cpo&vim

syn keyword pkgmanTodo     contained TODO FIXME XXX NOTE
syn region  pkgmanComment  display oneline start="^\s*#" end="$"
                           \ contains=pkgmanTodo,@Spell
syn region pkgmanLine      display oneline start="^[^ #]" end="$"
                           \ contains=pkgmanKey
syn keyword pkgmanKey      contained pkgsrcdir runscripts preferhigher
                           \ useregex readme writelog logmode rmlog_on_success
                           \ logfile makecommand addcommand removecommand
                           \ runscriptcommand
                           \ nextgroup=pkgmanVal skipwhite
syn region pkgmanVal       display oneline start="\s" end="$"
                           \ contains=pkgmanSVal,pkgmanKVal
syn keyword pkgmanKVal     contained append overwrite enabled disabled yes no
                           \ verbose compact disabled
                           \ skipwhite
syn region  pkgmanSVal     matchgroup=pkgmanSVal start="\s.\+\s" end="$"
                           \ contains=pkgmanKVal

hi def link pkgmanTodo     Todo
hi def link pkgmanComment  Comment
hi def link pkgmanKey      Identifier
hi def link pkgmanKVal     Special
hi def link pkgmanSVal     String
hi def link pkgmanVal      String

let b:current_syntax = "pkgmanconf"

let &cpo = s:cpo_save
unlet s:cpo_save

" End of file.
