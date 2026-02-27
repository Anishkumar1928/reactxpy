# ReactXPy Error Code Standard

This document freeze-locks all officially recognized error prefixes emitted by the C++ ReactXPy compiler. These codes guarantee that IDE integrations (VSCode extensions), doc-site links, and autofix parsers will remain stable even if the accompanying message text evolves over time.

## 🔹 RX1xx — Lexer Errors

| Code  | Meaning                     |
| ----- | --------------------------- |
| RX100 | Unterminated string literal |
| RX101 | Invalid character           |
| RX102 | Invalid number literal      |
| RX103 | Invalid escape sequence     |
| RX104 | Unexpected EOF              |

## 🔹 RX2xx — Parser / Syntax Errors

| Code  | Meaning                          |
| ----- | -------------------------------- |
| RX200 | Expected identifier after import |
| RX201 | Expected function name           |
| RX202 | Unexpected closing JSX tag       |
| RX203 | Missing closing bracket          |
| RX204 | JSX tag mismatch                 |
| RX205 | Unterminated JSX tag             |
| RX206 | Missing colon                    |
| RX207 | Unexpected token                 |
| RX208 | Invalid expression               |
| RX209 | Invalid assignment target        |
| RX210 | Missing block indentation        |
| RX211 | Invalid operator usage           |
| RX212 | Return outside function          |

## 🔹 RX3xx — JSX Specific (optional split later)

| Code  | Meaning               |
| ----- | --------------------- |
| RX300 | Invalid JSX attribute |
| RX301 | JSX nesting error     |
| RX302 | JSX expression error  |

## 🔹 RX4xx — Semantic Errors (NEXT PHASE)

| Code  | Meaning               |
| ----- | --------------------- |
| RX400 | Undefined variable    |
| RX401 | Duplicate declaration |
| RX402 | Invalid scope access  |
| RX403 | Function not found    |

## 🔹 RX9xx — Compiler Internal Errors

| Code  | Meaning                 |
| ----- | ----------------------- |
| RX900 | Internal compiler error |
| RX901 | Unsupported syntax      |
