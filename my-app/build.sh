#!/bin/bash
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "${BLUE}🚀 Building PYSX App...${NC}"

mkdir -p dist
compiled_files=()

COMPILER="pysx"

if ! command -v "$COMPILER" >/dev/null 2>&1; then
    echo -e "${RED}❌ Error: pysx compiler not found. Please install the pysx pip package globally.${NC}"
    exit 1
fi

while IFS= read -r file; do
    FILENAME=$(basename -- "$file")
    BASENAME="${FILENAME%.*}"
    OUTPUT_PATH="dist/${BASENAME}.js"
    
    echo "⚡ Compiling: $file -> $OUTPUT_PATH"
    "$COMPILER" "$file" -o "$OUTPUT_PATH"
    
    compiled_files+=("$OUTPUT_PATH")
done < <(find src -name "*.pysx" -type f)

if [ ${#compiled_files[@]} -gt 0 ]; then
    echo -e "\n🔗 Linking into dist/bundle.js..."
    "$COMPILER" --bundle "${compiled_files[@]}" dist/bundle.js
    echo -e "\n${GREEN}✅ Success! Build complete.${NC}"
else
    echo -e "${RED}❌ Error: No .pysx files found in src directory.${NC}"
fi
