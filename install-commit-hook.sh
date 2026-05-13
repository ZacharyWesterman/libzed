#!/usr/bin/env bash

if ! git_dir=$(git rev-parse --git-dir 2>/dev/null); then
  echo "error: not a git repository" >&2
  exit 1
fi

hook="$git_dir/hooks/pre-commit"
mkdir -p "$(dirname "$hook")"
rm "$hook" -f

cat > "$hook" <<'EOF'
#!/usr/bin/env bash

if command -v clang-tidy &>/dev/null; then
  echo "Comparing code against style guide..."
  make try-format
else
  echo -e "\\e[33mWARNING\\e[0m: clang-tidy not found; skipping formatting check." >&2
fi
EOF

chmod +x "$hook"
echo "Installed Git pre-commit hook: $hook"
