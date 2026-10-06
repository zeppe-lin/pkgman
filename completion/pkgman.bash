# bash programmable completion for pkgman(1)
# See COPYING for license terms and COPYRIGHT for notices.

# Keep this file self-contained.  pkgman completion is installed in the
# standard bash-completion directory, but it does not depend on private
# helper functions from a particular bash-completion release.

_pkgman_commands='
dumpconfig
list
list-dup
list-nodependents
list-locked
list-orphans
printf
info
readme
path
isinst
current
diff
dep
rdep
search
dsearch
fsearch
install
update
remove
sysup
lock
unlock
ls
cat
edit'

_pkgman_generic_options='
-h
--help
-V
--version
--no-std-config
--config=
--config-set=
--config-append=
--config-prepend=
--root=
--'

# Values advertised by pkgman-install(8), pkgman-update(8), and
# pkgman-sysup(8) for the default pkgmk/pkgadd commands.  These are value
# completions for --margs=/--aargs=, not pkgman options themselves.
_pkgman_pkgmk_args='-f -um -im -uf -if -ns -kw'
_pkgman_pkgadd_args='-f'

_pkgman_is_command()
{
  local candidate

  for candidate in $_pkgman_commands; do
    [ "$candidate" = "$1" ] && return 0
  done

  return 1
}

_pkgman_command_options()
{
  case $1 in
  list)
    printf '%s\n' '-v -vv --path --all --depsort --regex --filter='
    ;;
  list-nodependents | list-locked)
    printf '%s\n' '-v -vv --path'
    ;;
  list-orphans)
    printf '%s\n' '--dependents'
    ;;
  printf)
    printf '%s\n' '--sort= --regex --filter='
    ;;
  info)
    printf '%s\n' '-v -vv'
    ;;
  diff)
    printf '%s\n' '--all --deps --full --regex --filter='
    ;;
  dep | rdep)
    printf '%s\n' '-v -vv --path --all --tree --recursive --full'
    ;;
  search | dsearch)
    printf '%s\n' '-v -vv --path --regex'
    ;;
  fsearch)
    printf '%s\n' '-v -vv --path --regex --full'
    ;;
  install | update)
    printf '%s\n' '
-v -vv --test --log --deps --depsort --force --group
--margs= -fr -um -im -uf -if -ns -kw
--aargs= -f -i --ignore=
--install-scripts --pre-install --post-install'
    ;;
  remove)
    printf '%s\n' '
-v -vv --test --log --deps --force --group --rargs= --ignore=
--remove-scripts --pre-remove --post-remove'
    ;;
  sysup)
    printf '%s\n' '
-v -vv --test --log --deps --depsort --force --group
--margs= -fr -um -im -uf -if -ns -kw
--aargs= -f -i --ignore=
--install-scripts --pre-install --post-install'
    ;;
  ls)
    printf '%s\n' '--path'
    ;;
  esac
}

# Run an informational pkgman query in the same configuration/root
# context as the command line being completed.  _pkgman_context is local
# to _pkgman(); Bash's dynamic scoping makes it visible here.
_pkgman_query()
{
  command pkgman "${_pkgman_context[@]}" "$@" 2>/dev/null
}

_pkgman_available_packages()
{
  _pkgman_query printf '%n\n'
}

_pkgman_installed_packages()
{
  local line

  while IFS= read -r line; do
    [ -n "$line" ] && printf '%s\n' "${line%% *}"
  done < <(_pkgman_query list)
}

_pkgman_outdated_packages()
{
  _pkgman_query diff
}

_pkgman_locked_packages()
{
  local line

  while IFS= read -r line; do
    [ -n "$line" ] && printf '%s\n' "${line%% *}"
  done < <(_pkgman_query list-locked)
}

_pkgman_readme_packages()
{
  local has_readme package

  while IFS=' ' read -r has_readme package; do
    if [ "$has_readme" = yes ] && [ -n "$package" ]; then
      printf '%s\n' "$package"
    fi
  done < <(_pkgman_query printf '%R %n\n')
}


_pkgman_complete_words()
{
  local words=$1 prefix=$2 reply

  COMPREPLY=($(compgen -W "$words" -- "$prefix" || true))
  for reply in "${COMPREPLY[@]}"; do
    if [[ $reply == *= ]]; then
      compopt -o nospace 2>/dev/null || true
      break
    fi
  done
}

_pkgman_complete_lines()
{
  local prefix=$1 line

  while IFS= read -r line; do
    case $line in
    "$prefix"*) COMPREPLY+=("$line") ;;
    esac
  done
}

_pkgman_complete_available()
{
  _pkgman_complete_lines "$1" < <(_pkgman_available_packages)
}

_pkgman_complete_installed()
{
  _pkgman_complete_lines "$1" < <(_pkgman_installed_packages)
}

_pkgman_complete_outdated()
{
  _pkgman_complete_lines "$1" < <(_pkgman_outdated_packages)
}

_pkgman_complete_locked()
{
  _pkgman_complete_lines "$1" < <(_pkgman_locked_packages)
}

_pkgman_complete_readmes()
{
  _pkgman_complete_lines "$1" < <(_pkgman_readme_packages)
}

_pkgman_complete_source_file()
{
  local package=$1 prefix=$2

  _pkgman_complete_lines "$prefix" < <(_pkgman_query ls "$package")
}

# Bash builds COMP_WORDS using COMP_WORDBREAKS before invoking this
# function.  '=' is a word break by default, so --root=/mnt is presented
# as three words: --root, =, /mnt.  Reassemble selected word-break
# characters locally rather than modifying the global COMP_WORDBREAKS,
# which would affect unrelated completion functions.
_pkgman_reassemble_comp_words()
{
  local exclude=$1 first i j

  _pkgman_words=()
  _pkgman_cword=$COMP_CWORD

  # Ignore requested characters that are not actual word breaks in this
  # shell.  This also makes the helper safe with customized
  # COMP_WORDBREAKS values.
  exclude=${exclude//[^$COMP_WORDBREAKS]}
  if [ -z "$exclude" ]; then
    _pkgman_words=("${COMP_WORDS[@]}")
    return
  fi

  for ((i = 0, j = 0; i < ${#COMP_WORDS[@]}; ++i, ++j)); do
    first=1
    while [ $i -gt 0 ] && [ -n "${COMP_WORDS[i]}" ] &&
      [ "${COMP_WORDS[i]//[^$exclude]}" = "${COMP_WORDS[i]}" ]; do
      # Join a separator to the previous token, except immediately after
      # the command name where there is no option/value token to join.
      if [ $j -ge 2 ] && [ -n "$first" ]; then
        ((j -= 1))
      fi
      first=
      _pkgman_words[j]=${_pkgman_words[j]-}${COMP_WORDS[i]}
      if [ $i -eq $COMP_CWORD ]; then
        _pkgman_cword=$j
      fi

      if [ $i -lt $((${#COMP_WORDS[@]} - 1)) ]; then
        ((i += 1))
      else
        return
      fi
    done

    _pkgman_words[j]=${_pkgman_words[j]-}${COMP_WORDS[i]}
    if [ $i -eq $COMP_CWORD ]; then
      _pkgman_cword=$j
    fi
  done
}

_pkgman_complete_attached_path()
{
  local value=${1#*=} match
  local mode=$2

  COMPREPLY=()
  if [ "$mode" = directory ]; then
    while IFS= read -r match; do
      COMPREPLY+=("${match%/}/")
    done < <(compgen -d -- "$value")
    compopt -o filenames -o nospace 2>/dev/null || true
  else
    while IFS= read -r match; do
      COMPREPLY+=("$match")
    done < <(compgen -f -- "$value")
    compopt -o filenames 2>/dev/null || true
  fi
}

_pkgman_complete_attached_words()
{
  local words=$1 value=${2#*=}

  COMPREPLY=($(compgen -W "$words" -- "$value" || true))
}

_pkgman_complete_ignore()
{
  local command=$1 value=${2#--ignore=} head prefix package

  if [[ $value == *,* ]]; then
    head=${value%,*},
    prefix=${value##*,}
  else
    head=
    prefix=$value
  fi

  case $command in
  remove)
    while IFS= read -r package; do
      case ,$head in
      *,$package,*) continue ;;
      esac
      case $package in
      "$prefix"*) COMPREPLY+=("$head$package") ;;
      esac
    done < <(_pkgman_installed_packages)
    ;;
  *)
    while IFS= read -r package; do
      case ,$head in
      *,$package,*) continue ;;
      esac
      case $package in
      "$prefix"*) COMPREPLY+=("$head$package") ;;
      esac
    done < <(_pkgman_available_packages)
    ;;
  esac

  # Keep the cursor attached to the value so the user can append a comma
  # and immediately complete another package name.
  compopt -o nospace 2>/dev/null || true
}

_pkgman()
{
  local cur command= package= word options
  local command_index=-1 position=0 endopts=0 top_endopts=0
  local context_endopts=0
  local i
  local -a _pkgman_context=() _pkgman_words=()
  local _pkgman_cword=0

  COMPREPLY=()
  _pkgman_reassemble_comp_words '='
  cur=${_pkgman_words[_pkgman_cword]}

  # Preserve generic configuration selectors for all helper queries.
  # Once -- appears, the parser treats every following word literally.
  for ((i = 1; i < _pkgman_cword; ++i)); do
    word=${_pkgman_words[i]}

    if [ "$word" = -- ]; then
      context_endopts=1
      continue
    fi

    if [ $context_endopts -eq 0 ]; then
      case $word in
      --no-std-config | --config=* | --config-set=* | \
      --config-append=* | --config-prepend=* | --root=*)
        _pkgman_context+=("$word")
        ;;
      esac
    fi
  done

  # The command is the first non-option word before the current word.
  # Generic options may legally precede it.
  for ((i = 1; i < _pkgman_cword; ++i)); do
    word=${_pkgman_words[i]}

    if [ $top_endopts -eq 0 ]; then
      if [ "$word" = -- ]; then
        top_endopts=1
        continue
      fi

      [[ $word == -* ]] && continue
    fi

    command=$word
    command_index=$i
    break
  done

  # Complete the command itself, including the case where generic
  # options precede it.
  if [ $command_index -lt 0 ]; then
    if [[ $cur == --config=* ]]; then
      _pkgman_complete_attached_path "$cur" file
      return 0
    fi

    if [[ $cur == --root=* ]]; then
      _pkgman_complete_attached_path "$cur" directory
      return 0
    fi

    if [ $top_endopts -eq 1 ]; then
      _pkgman_complete_words "$_pkgman_commands" "$cur"
    else
      _pkgman_complete_words \
        "$_pkgman_commands $_pkgman_generic_options" "$cur"
    fi
    return 0
  fi

  _pkgman_is_command "$command" || return 0
  endopts=$top_endopts

  # Determine whether option parsing has been terminated and count
  # positional arguments before the current word.  pkgman options that
  # take values use --option=value, so no following word is consumed.
  for ((i = command_index + 1; i < _pkgman_cword; ++i)); do
    word=${_pkgman_words[i]}

    if [ $endopts -eq 0 ]; then
      if [ "$word" = -- ]; then
        endopts=1
        continue
      fi

      [[ $word == -* ]] && continue
    fi

    if [ $position -eq 0 ]; then
      package=$word
    fi
    ((position += 1))
  done

  if [ $endopts -eq 0 ]; then
    if [[ $cur == --config=* ]]; then
      _pkgman_complete_attached_path "$cur" file
      return 0
    fi

    if [[ $cur == --root=* ]]; then
      _pkgman_complete_attached_path "$cur" directory
      return 0
    fi

    case $command in
    install | update | sysup)
      if [[ $cur == --ignore=* ]]; then
        _pkgman_complete_ignore "$command" "$cur"
        return 0
      fi

      if [[ $cur == --margs=* ]]; then
        _pkgman_complete_attached_words "$_pkgman_pkgmk_args" "$cur"
        return 0
      fi

      if [[ $cur == --aargs=* ]]; then
        _pkgman_complete_attached_words "$_pkgman_pkgadd_args" "$cur"
        return 0
      fi
      ;;
    remove)
      if [[ $cur == --ignore=* ]]; then
        _pkgman_complete_ignore "$command" "$cur"
        return 0
      fi
      ;;
    esac

    if [[ $cur == -* ]]; then
      options="$(_pkgman_command_options "$command") \
$_pkgman_generic_options"
      _pkgman_complete_words "$options" "$cur"
      return 0
    fi
  fi

  # Positional arguments are command-specific.  Commands without a
  # case below intentionally offer no positional completion.
  case $command in
  install)
    _pkgman_complete_available "$cur"
    ;;
  ls)
    [ $position -eq 0 ] && _pkgman_complete_available "$cur"
    ;;
  info | path | dep | rdep)
    [ $position -eq 0 ] && _pkgman_complete_available "$cur"
    ;;
  readme)
    [ $position -eq 0 ] && _pkgman_complete_readmes "$cur"
    ;;
  isinst | remove | lock)
    _pkgman_complete_installed "$cur"
    ;;
  current)
    [ $position -eq 0 ] && _pkgman_complete_installed "$cur"
    ;;
  update)
    _pkgman_complete_outdated "$cur"
    ;;
  unlock)
    _pkgman_complete_locked "$cur"
    ;;
  cat | edit)
    if [ $position -eq 0 ]; then
      _pkgman_complete_available "$cur"
    elif [ $position -eq 1 ] && [ -n "$package" ]; then
      _pkgman_complete_source_file "$package" "$cur"
    fi
    ;;
  esac

  return 0
}

complete -F _pkgman pkgman

# vim: ft=bash
# End of file.
