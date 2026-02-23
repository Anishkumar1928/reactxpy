#!/bin/bash

GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "${BLUE}🚀 Starting PYSX Compiler Build...${NC}"

# 1. Compile the C++ toolset from the src folder (where your .cpp files are)
echo "⚙️  Building C++ Toolset..."
g++ src/main.cpp src/parser.cpp src/lexer.cpp src/generator.cpp src/linker.cpp -o pysx

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ C++ Compilation failed!${NC}"
    exit 1
fi

echo -e "\n${GREEN}✅ Success! Compiled PYSX Compiler into: ./pysx${NC}"