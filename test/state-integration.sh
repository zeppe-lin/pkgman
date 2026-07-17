#!/bin/sh
set -eu

pkgman=$1
tmp=$(mktemp -d "${TMPDIR:-/tmp}/pkgman-state.XXXXXX")
trap 'rm -rf "$tmp"' EXIT HUP INT TERM

root=$tmp/root
db=$root/var/lib/pkg/db
mkdir -p "$(dirname "$db")"

cat > "$db" <<'EOF'
alpha
1.2-3
usr/bin/alpha
usr/share/alpha/

beta
2.0-1

EOF

actual=$(
  "$pkgman" --root="$root" --no-std-config current alpha
)
[ "$actual" = '1.2-3' ] || {
  echo "current: expected 1.2-3, got: $actual" >&2
  exit 1
}

actual=$(
  "$pkgman" --root="$root" --no-std-config isinst alpha beta
)
expected="package 'alpha' is installed
package 'beta' is installed"
[ "$actual" = "$expected" ] || {
  echo "isinst: unexpected output:" >&2
  printf '%s\n' "$actual" >&2
  exit 1
}

if "$pkgman" --root="$root" --no-std-config current missing \
    >"$tmp/missing.out" 2>"$tmp/missing.err"; then
  echo 'current: absent package unexpectedly succeeded' >&2
  exit 1
fi

grep -Fqx "pkgman: package 'missing' not installed." "$tmp/missing.err"

printf 'broken\n' > "$db"
if "$pkgman" --root="$root" --no-std-config current alpha \
    >"$tmp/broken.out" 2>"$tmp/broken.err"; then
  echo 'current: malformed state unexpectedly succeeded' >&2
  exit 1
fi

grep -Fq 'pkgman: installed state:' "$tmp/broken.err"
grep -Fq 'package record has no version line' "$tmp/broken.err"
