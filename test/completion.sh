#!/bin/bash
# Test the installed-style Bash completion without requiring a pkgman
# database or package source tree.

set -eu

completion=$1
tmpdir=$(mktemp -d)
trap 'rm -rf "$tmpdir"' EXIT

mkdir -p \
  "$tmpdir/bin" \
  "$tmpdir/root-one/child-one" \
  "$tmpdir/root-two"
touch \
  "$tmpdir/root-file" \
  "$tmpdir/root-one/child-file" \
  "$tmpdir/config-one" \
  "$tmpdir/config-two"

cat > "$tmpdir/bin/pkgman" <<'STUB'
#!/bin/bash

printf '%q ' "$@" >> "$PKGMAN_COMPLETION_TEST_LOG"
printf '\n' >> "$PKGMAN_COMPLETION_TEST_LOG"

command=
for arg in "$@"; do
  case $arg in
  dumpconfig | list | list-dup | list-nodependents | list-locked | \
  list-orphans | printf | info | readme | path | isinst | current | \
  diff | dep | rdep | search | dsearch | fsearch | install | update | \
  remove | sysup | lock | unlock | ls | cat | edit)
    command=$arg
    break
    ;;
  esac
done

case $command in
printf)
  case ${@: -1} in
  '%n\n')
    printf 'alpha\nbeta\ngamma\n'
    ;;
  '%R %n\n')
    printf 'yes alpha\nno beta\nyes gamma\n'
    ;;
  esac
  ;;
list)
  printf 'alpha\norphan (not found in the packages sources tree)\n'
  ;;
diff)
  printf 'beta\n'
  ;;
list-locked)
  printf 'alpha\n'
  ;;
ls)
  printf 'Pkgfile\nREADME.md\nfix.patch\n'
  ;;
esac
STUB
chmod +x "$tmpdir/bin/pkgman"

export PATH="$tmpdir/bin:$PATH"
export PKGMAN_COMPLETION_TEST_LOG="$tmpdir/queries"
: > "$PKGMAN_COMPLETION_TEST_LOG"

# shellcheck source=/dev/null
source "$completion"

complete_line()
{
  COMPREPLY=()
  COMP_WORDS=("$@")
  COMP_CWORD=$((${#COMP_WORDS[@]} - 1))
  _pkgman
}

reply_contains()
{
  local expected=$1 reply

  for reply in "${COMPREPLY[@]}"; do
    [ "$reply" = "$expected" ] && return 0
  done

  printf 'missing completion: %s\n' "$expected" >&2
  printf 'reply:' >&2
  printf ' <%s>' "${COMPREPLY[@]}" >&2
  printf '\n' >&2
  return 1
}

reply_excludes()
{
  local unexpected=$1 reply

  for reply in "${COMPREPLY[@]}"; do
    if [ "$reply" = "$unexpected" ]; then
      printf 'unexpected completion: %s\n' "$unexpected" >&2
      return 1
    fi
  done
}

# Top-level grammar and generic options.
complete_line pkgman ''
reply_contains install
reply_contains readme
reply_contains --config-prepend=
reply_contains --root=

complete_line pkgman -- ''
reply_contains install
reply_excludes --root=

complete_line pkgman -- install -
[ ${#COMPREPLY[@]} -eq 0 ]

# Command options come from the command manuals, plus generic options.
complete_line pkgman list --d
reply_contains --depsort

complete_line pkgman remove --f
reply_contains --force

complete_line pkgman sysup --f
reply_contains --force

complete_line pkgman install -i
reply_contains -i

complete_line pkgman list-dup --p
reply_excludes --prefer-higher

# Positional completion uses the appropriate package population.
complete_line pkgman install a
reply_contains alpha

complete_line pkgman readme g
reply_contains gamma
reply_excludes beta

complete_line pkgman remove o
reply_contains orphan

complete_line pkgman update b
reply_contains beta

complete_line pkgman unlock a
reply_contains alpha

# File commands complete package source entries after the package name.
complete_line pkgman cat alpha f
reply_contains fix.patch

complete_line pkgman ls alpha ''
[ ${#COMPREPLY[@]} -eq 0 ]

# Bash splits attached values at '=' before invoking completion.  Exercise
# the real COMP_WORDS shape instead of fabricating --option=value as one
# word.  Bare values must already produce candidates; Readline decides
# whether a single ambiguous Tab displays them immediately.
complete_line pkgman install --ignore =
reply_contains alpha
reply_contains beta
reply_contains gamma

complete_line pkgman install --ignore = alpha,g
reply_contains alpha,gamma

# Ignore completion remains attached so another comma can start the next
# package, and already-selected packages are not offered again.
complete_line pkgman install --ignore = alpha,
reply_contains alpha,beta
reply_contains alpha,gamma
reply_excludes alpha,alpha

# Delegated pkgmk/pkgadd argument values complete the documented flags,
# including the empty attached-value state.
complete_line pkgman install --margs =
reply_contains -f
reply_contains -um
reply_contains -kw

complete_line pkgman install --margs = -u
reply_contains -um
reply_contains -uf

complete_line pkgman install --aargs =
reply_contains -f

complete_line pkgman install --aargs = -
reply_contains -f

# Helper queries inherit generic configuration selectors from the line.
: > "$PKGMAN_COMPLETION_TEST_LOG"
complete_line pkgman --root = /alternate install g
reply_contains gamma
grep -F -- '--root=/alternate printf' \
  "$PKGMAN_COMPLETION_TEST_LOG" >/dev/null

# Queries whose candidate population comes from installed package state must
# consult the selected root as well.
: > "$PKGMAN_COMPLETION_TEST_LOG"
complete_line pkgman --root = /alternate remove a
reply_contains alpha
grep -F -- '--root=/alternate list' \
  "$PKGMAN_COMPLETION_TEST_LOG" >/dev/null

: > "$PKGMAN_COMPLETION_TEST_LOG"
complete_line pkgman update --root = /alternate b
reply_contains beta
grep -F -- '--root=/alternate diff' \
  "$PKGMAN_COMPLETION_TEST_LOG" >/dev/null

: > "$PKGMAN_COMPLETION_TEST_LOG"
complete_line pkgman --root = /alternate unlock a
reply_contains alpha
grep -F -- '--root=/alternate list-locked' \
  "$PKGMAN_COMPLETION_TEST_LOG" >/dev/null

: > "$PKGMAN_COMPLETION_TEST_LOG"
complete_line pkgman remove --root = /alternate --ignore = a
reply_contains alpha
grep -F -- '--root=/alternate list' \
  "$PKGMAN_COMPLETION_TEST_LOG" >/dev/null

# Attached path options are completed as paths, not pkgman tokens.  Directory
# candidates stay unsuffixed in COMPREPLY; Readline adds the slash because the
# completion function enables the filenames option.
complete_line pkgman --root = "$tmpdir/root-"
reply_contains "$tmpdir/root-one"
reply_contains "$tmpdir/root-two"
reply_excludes "$tmpdir/root-file"

complete_line pkgman --root = "$tmpdir/root-one/ch"
reply_contains "$tmpdir/root-one/child-one"
reply_excludes "$tmpdir/root-one/child-file"

complete_line pkgman --config = "$tmpdir/config-"
reply_contains "$tmpdir/config-one"
reply_contains "$tmpdir/config-two"

exit 0
