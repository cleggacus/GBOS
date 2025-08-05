#!/usr/bin/env bash

GBDK_INCLUDE="$GBDK_HOME/include"
PROJECT_DIR="$(pwd)"
OUTPUT="compile_commands.json"

echo "[" > "$OUTPUT"

first=1
for f in $(find src res -type f -name '*.c'); do
  if [ $first -eq 0 ]; then
    echo "," >> "$OUTPUT"
  fi
  first=0

  cat >> "$OUTPUT" << EOF
  {
    "directory": "$PROJECT_DIR",
    "command": "gcc -I$GBDK_INCLUDE -Isrc -Ires -Iinclude -D__PORT_sm83 -D__TARGET_gb -c $f",
    "file": "$f"
  }
EOF
done

echo "]" >> "$OUTPUT"

echo "compile_commands.json generated with $(grep -c '"file"' $OUTPUT) entries"
