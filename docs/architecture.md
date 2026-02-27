# ReactXPy Architecture

## Overview

ReactXPy is a Python-to-JavaScript transpiler that enables developers to write React components using Python syntax. The architecture consists of a high-performance C++ compiler core, a Python CLI wrapper, and a lightweight runtime system.

## System Architecture

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                           ReactXPy System                                   │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐                  │
│  │   Source     │───▶│   Compiler   │───▶│   Output     │                  │
│  │   (.pysx)    │    │   (C++)      │    │   (.js)      │                  │
│  └──────────────┘    └──────────────┘    └──────────────┘                  │
│         │                   │                   │                          │
│         ▼                   ▼                   ▼                          │
│  ┌─────────────────────────────────────────────────────┐                   │
│  │              Python CLI & Tooling                    │                   │
│  │  ┌──────────┐  ┌──────────┐  ┌──────────┐          │                   │
│  │  │  reactxpy│  │create-   │  │  dev.py  │          │                   │
│  │  │  command │  │reactxpy- │  │  (server)│          │                   │
│  │  │          │  │app       │  │          │          │                   │
│  │  └──────────┘  └──────────┘  └──────────┘          │                   │
│  └─────────────────────────────────────────────────────┘                   │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

## Core Components

### 1. C++ Compiler (`/compiler/`)

The compiler is the heart of ReactXPy, written in C++17 for maximum performance.

#### 1.1 Lexical Analyzer (`lexer.cpp`, `lexer.h`)

**Purpose:** Tokenizes Python-style source code into a stream of tokens.

**Key Features:**
- Python PEP-8 indentation tracking (INDENT/DEDENT tokens)
- JSX tag recognition (TAG_OPEN, TAG_CLOSE, SELF_CLOSE)
- String literal support (single, double, backtick, triple quotes)
- JavaScript literal recognition (true/false/null/None)
- Comment handling (Python `#` style)
- Position tracking for error reporting (line/column)

**Token Types:**
```cpp
enum TokenType {
    // Keywords
    DEF, RETURN, IMPORT, FROM,
    // Python Scope
    INDENT, DEDENT,
    // Identifiers & Literals
    IDENTIFIER, STRING, TRUE, FALSE, NULLTOK,
    // JSX
    TAG_OPEN, TAG_CLOSE, SELF_CLOSE,
    ATTRIBUTE_NAME, ATTRIBUTE_VALUE, ATTRIBUTE_EXPR,
    // Expressions
    EXPRESSION, TEXT,
    // Syntax
    LPAREN, RPAREN, COLON, COMMA, LBRACKET, RBRACKET,
    EQUAL, STAR, END
};
```

#### 1.2 Parser (`parser.cpp`, `parser.h`)

**Purpose:** Builds an Abstract Syntax Tree (AST) from tokens.

**Parsing Strategy:**
- Recursive descent parsing
- Top-down approach for function and JSX structures
- Error recovery with continuation

**AST Nodes:**
```cpp
struct JSXNode {
    std::string tag;
    std::map<std::string, PropValue> props;
    std::vector<JSXChild> children;
    bool selfClosing;
};

struct FunctionNode {
    std::string name;
    std::vector<std::string> params;
    std::vector<std::shared_ptr<JSXNode>> jsx_list;
    std::string body;
};

struct ImportNode {
    ImportType type;
    std::string module;
};

struct ProgramNode {
    std::vector<ImportNode> imports;
    std::vector<FunctionNode> functions;
};
```

#### 1.3 Code Generator (`generator.cpp`, `generator.h`)

**Purpose:** Transforms AST into JavaScript ES6 modules.

**Transformations:**
- Python `lambda` → JavaScript arrow functions `() =>`
- Python keywords → JS equivalents (`True`→`true`, `None`→`null`)
- `useState` tuple unpacking → `const [val, setVal] = ReactXPy.useState()`
- `useEffect` → `ReactXPy.useEffect()`
- JSX → `React.createElement()` calls
- `Class` attribute → `className`
- ES6 module exports

**Example Transformation:**
```python
# Input (.pysx)
def Counter():
    count, setCount = useState(0)
    return <button onClick={lambda: setCount(count + 1)}>{count}</button>
```

```javascript
// Output (.js)
function Counter(props){
  const [count, setCount] = ReactXPy.useState(0);
  return React.createElement("button", { onClick: () => setCount(count + 1) }, count);
}
export default Counter;
```

#### 1.4 Linker/Bundler (`linker.cpp`, `linker.h`)

**Purpose:** Combines multiple JS files into a single bundle.

**Usage:**
```bash
./pysx --bundle file1.js file2.js output.js
```

#### 1.5 Error Reporter (`errors.cpp`, `errors.h`)

**Purpose:** Provides detailed error messages with source context.

**Error Codes:**
| Code | Type | Description |
|------|------|-------------|
| RX100 | SyntaxError | Unterminated string literal |
| RX101 | SyntaxError | Invalid character |
| RX200 | SyntaxError | Expected identifier after import |
| RX201 | SyntaxError | Expected function name |
| RX202 | SyntaxError | Unexpected closing tag |
| RX203 | SyntaxError | Incomplete assignment |
| RX204 | SyntaxError | Mismatched closing tag |
| RX205 | SyntaxError | Unterminated JSX tag |
| RX206 | SyntaxError | Empty attribute value |
| RX207 | SyntaxError | Expected opening tag |
| RX208 | SyntaxError | Incomplete if statement |
| RX209 | SyntaxError | Empty lambda expression |

### 2. Python Package (`/reactxpy/`)

#### 2.1 CLI Wrapper (`cli.py`)

**Purpose:** Provides the `reactxpy` command-line interface.

**Functionality:**
- Locates the compiled C++ binary
- Passes arguments to the compiler
- Handles version flags

#### 2.2 Project Scaffolder (`create_app.py`)

**Purpose:** Generates new ReactXPy projects.

**Command:**
```bash
create-reactxpy-app my-app
```

**Generated Structure:**
```
my-app/
├── src/
│   ├── components/
│   └── App.pysx
├── public/
│   ├── index.html
│   └── style.css
├── dist/
├── runtime/
├── build.py
└── dev.py
```

### 3. Development Tools

#### 3.1 Build Script (`build.py`)

**Purpose:** Compiles all `.pysx` files in `src/` to `dist/`.

**Process:**
1. Scans `src/` for `.pysx` files
2. Invokes `reactxpy` compiler for each file
3. Outputs ES6 modules to `dist/`

#### 3.2 Dev Server (`dev.py`)

**Purpose:** Hot-reload development server.

**Features:**
- File watcher for `.pysx` changes
- Auto-recompilation
- HTTP server on localhost:3000 (or next available)
- Version endpoint for cache busting

### 4. Runtime (`/runtime/runtime.js`)

**Purpose:** Lightweight React hook bridge.

**Current Implementation:** Empty (React hooks used directly)

**Future:** May include Python-compatible utility functions

## Data Flow

```
┌─────────┐     ┌──────────┐     ┌─────────┐     ┌──────────┐     ┌─────────┐
│  .pysx  │────▶│  Lexer   │────▶│  Parser │────▶│ Generator│────▶│   .js   │
│  Source │     │ (Tokens) │     │  (AST)  │     │  (JS)    │     │  Output │
└─────────┘     └──────────┘     └─────────┘     └──────────┘     └─────────┘
     │                │               │               │                │
     │                │               │               │                │
     ▼                ▼               ▼               ▼                ▼
┌──────────────────────────────────────────────────────────────────────────┐
│                         Error Reporter                                   │
│                    (Line/Column tracking)                                │
└──────────────────────────────────────────────────────────────────────────┘
```

## Compilation Pipeline

### Phase 1: Lexical Analysis
1. Read source file character by character
2. Recognize Python indentation (spaces/tabs → INDENT/DEDENT)
3. Identify keywords, identifiers, operators
4. Parse JSX tags and attributes
5. Handle string literals with escape sequences

### Phase 2: Parsing
1. Build ProgramNode from token stream
2. Parse import statements
3. Parse function definitions with parameters
4. Parse function body with JSX placeholders
5. Parse JSX tree structure recursively

### Phase 3: Code Generation
1. Generate import statements
2. For each function:
   - Transform Python syntax to JavaScript
   - Convert `useState` destructuring
   - Convert `useEffect` calls
   - Replace JSX placeholders with `React.createElement()`
   - Generate function body with proper indentation
3. Add ES6 export statement

### Phase 4: Output
1. Write generated JavaScript to output file
2. Report success or errors

## Module System

ReactXPy uses ES6 modules:

**Import:**
```python
import ComponentName
```

**Generated:**
```javascript
import ComponentName from "./ComponentName.js";
```

**Export:**
- Last function in file is automatically exported as default

## Testing Architecture

### Error Tests (`run_error_tests.py`)
- 17 tests for syntax error detection
- Validates error codes and messages
- Located in `tests/error_tests/`

### Valid Tests (`run_valid_tests.py`)
- 50 tests for normal programming scenarios
- Ensures valid code compiles correctly
- Located in `tests/valid_tests/`

## Build System

### Compilation
```bash
g++ -std=c++17 -o reactxpy_compiler \
    main.cpp lexer.cpp parser.cpp generator.cpp linker.cpp errors.cpp
```

### Installation
```bash
pip install reactxpy
```

The setup.py compiles the C++ binary during installation.

## Performance Characteristics

- **Lexer:** O(n) where n = source length
- **Parser:** O(n) single pass with recursive descent
- **Generator:** O(n) AST traversal
- **Memory:** O(n) for token stream and AST
- **Compilation:** ~1ms per 1000 lines (estimated)

## Future Enhancements

1. **Language Server Protocol (LSP)** for IDE integration
2. **Source maps** for debugging
3. **Type hints** to PropTypes conversion
4. **Async/await** support
5. **List comprehensions** for JSX rendering
6. **CSS-in-Python** support

## File Organization

```
pysx/
├── compiler/           # C++ transpiler source
│   ├── main.cpp        # Entry point
│   ├── lexer.cpp/h     # Tokenizer
│   ├── parser.cpp/h    # AST builder
│   ├── generator.cpp/h # Code generator
│   ├── linker.cpp/h    # Bundler
│   └── errors.cpp/h    # Error reporting
├── reactxpy/           # Python package
│   ├── cli.py          # Command wrapper
│   ├── create_app.py   # Project generator
│   └── reactxpy_compiler # Compiled binary
├── runtime/            # JS runtime (minimal)
├── tests/              # Test suites
│   ├── error_tests/    # Error validation tests
│   └── valid_tests/    # Valid code tests
├── playground/         # Example projects
└── docs/               # Documentation
```
