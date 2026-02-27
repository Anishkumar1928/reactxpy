# ReactXPy Syntax Guide

ReactXPy is a syntax paradigm that allows you to write interactive frontend applications using native Python syntax coupled with HTML (JSX).

Under the hood, ReactXPy bypasses Node.js entirely and utilizes a lightning-fast native transpiler to convert your code into ES6 JavaScript mapped directly to React DOM components.

## Table of Contents

1. [Defining Components](#1-defining-components)
2. [React Hooks](#2-using-react-hooks-usestate-useeffect)
3. [Python Lambdas](#3-arrow-functions--python-lambdas)
4. [JSX Expressions](#4-evaluated-jsx-expressions-)
5. [Primitive Transpilation](#5-primitive-transpilation)
6. [CSS Class Names](#6-css-class-name-mapping)
7. [Control Flow](#7-control-flow)
8. [List Rendering](#8-list-rendering)
9. [Imports](#9-cross-module-importing)
10. [Error Codes](#10-error-codes)
11. [Limitations](#11-known-edge-cases--limitations)

---

## 1. Defining Components

Components in ReactXPy are exactly identical to Python `def` functions.

- Component names **MUST** start with a Capital letter (e.g., `def App():`, `def Header(props):`).
- Components **MUST** `return` a root HTML element or another Component.
- The transpiler natively recognizes `<` and `>` tags to automatically pivot from Pythonic evaluation into React Virtual DOM evaluation.

```python
# Button.reactxpy
def Button(label):
    # Pure Python scope
    button_name = label + " Click Me!"

    # Returning a JSX node
    return <button class="btn-primary">
        <span>{button_name}</span>
    </button>
```

---

## 2. Using React Hooks (`useState`, `useEffect`)

ReactXPy v2 supports deep React Hook interop.

### `useState`

State destructuring mimics Pythonic tuple unpacking perfectly. The transpiler maps standard Python assignment cleanly onto `React.useState()`.

```python
def Counter():
    # Native Python Tuple Parsing -> const [count, setCount] = ReactXPy.useState(0);
    count, setCount = useState(0)

    # Note the inline Lambda callback instead of a standard arrow function!
    return <div class="counter-box">
        <b>Hits: {count}</b>
        <button onClick={lambda: setCount(count + 1)}>Increment</button>
    </div>
```

### `useEffect`

Side effects evaluate using standard Python lists representing the dependency arrays.

```python
def DocumentUpdater():
    title, setTitle = useState("ReactXPy")

    # Binds directly to React.useEffect, listening for changes to the [title] array
    useEffect(lambda: document.title = title, [title])

    return <input value={title} onChange={lambda e: setTitle(e.target.value)} />
```

---

## 3. Arrow Functions → Python Lambdas

The most critical departure from standard Javascript is event handling. Because standard JavaScript arrow functions `() => {}` violate the Python parser, ReactXPy leverages **Python Lambdas** instead.

The native C++ Lexer deeply transforms inline Python `lambda` arguments over to Javascript arrow functions within the AST.

```python
# ❌ INCORRECT (Invalid Python Syntax)
# <button onClick={() => console.log("clicked")}>

# ✅ CORRECT (Valid ReactXPy Syntax)
# <button onClick={lambda: console.log("clicked")}>
```

You can also pass arguments through Lambdas precisely as you would in Python:

```python
# Passes the 'event' (e) parameter downstream
<input onChange={lambda e: console.log(e.target.value)} />
```

If you require multi-line logic for an event handler, declare a nested function inside the component block instead of an inline lambda:

```python
def LoginScreen():
    username, setUsername = useState("")

    def handleSubmit():
        if (username == "admin"):
            alert("Welcome!")
        else:
            alert("Denied.")

    # Reference the internal function pointer
    return <button onClick={handleSubmit}>Login</button>
```

---

## 4. Evaluated JSX Expressions (`{}`)

Whenever you are inside an HTML layout (between tags), you can write `{ ... }` curly braces to escape back into Python/JavaScript evaluation logic.

**Note on Logic Conversion:**
ReactXPy is deeply intertwined with Javascript's runtime execution. Therefore, within `{}` blocks, you are evaluating mathematical, boolean, and logical operators that compile into the browser.

### Ternary / Conditional Operators

ReactXPy parses standard JavaScript ternary structures instead of Python's `A if X else B` structure for inline DOM conditionals.

```python
{ isLoggedIn ? <h1>Welcome Back!</h1> : <h1>Please Sign In</h1> }
```

### And (&&) Short Circuiting

You can natively render nodes using the standard logical AND operator wrapper:

```python
{ hasErrors && <div class="error-modal">Build Failed</div> }
```

---

## 5. Primitive Transpilation

The ReactXPy AST automatically converts Pythonic keywords into their Javascript string-literal analogs for you. You do not need to alter your standard Pythonic variable states.

- `True` transpiles instantly to `true`
- `False` transpiles instantly to `false`
- `None` transpiles instantly to `null`

```python
# Before Compilation (ReactXPy)
def FeatureFlag():
    isEnabled = True
    sessionObj = None

# After Compilation (dist/bundle.js)
# function FeatureFlag() {
#    let isEnabled = true;
#    let sessionObj = null;
# }
```

---

## 6. CSS Class Name Mapping

To maintain parity with standard HTML documentation, ReactXPy allows you to use standard `class="foo"` attributes directly on tags.

You **do not** need to type `className="foo"`. The underlying Generator parses the standard `class` attribute and safely maps it into the React DOM standard property.

```python
# You write this:
<div class="card-outline">

# The framework natively mounts this to the React DOM tree:
# React.createElement("div", { className: "card-outline" })
```

---

## 7. Control Flow

### If Statements

Standard Python if statements work within component bodies:

```python
def Greeting(user):
    if user:
        return <h1>Hello, {user.name}!</h1>
    return <h1>Hello, Stranger!</h1>
```

### Conditional Rendering in JSX

Use JavaScript ternary operators for inline conditions:

```python
{ isLoggedIn ? <h1>Welcome Back!</h1> : <h1>Please Sign In</h1> }
```

Use logical AND for conditional display:

```python
{ hasErrors && <div class="error-modal">Build Failed</div> }
```

### While and For Loops

Standard Python loops work in component logic:

```python
def NumberList():
    numbers = []
    i = 0
    while i < 5:
        numbers.append(i)
        i += 1
    return <div>{numbers.join(", ")}</div>
```

---

## 8. List Rendering

### Using map()

Render lists using JavaScript's `map()` method:

```python
def TodoList():
    todos = [
        {id: 1, text: "Learn ReactXPy", done: false},
        {id: 2, text: "Build an app", done: false}
    ]
    
    return <ul>
        {todos.map(lambda todo: 
            <li key={todo.id}>
                {todo.text}
            </li>
        )}
    </ul>
```

### With Index

```python
{items.map(lambda item, index: 
    <li key={index}>{index + 1}. {item}</li>
)}
```

### Filter and Map

```python
{users
    .filter(lambda u: u.active)
    .map(lambda u: <span key={u.id}>{u.name}</span>)
}
```

---

## 9. Cross-Module Importing

ReactXPy flattens imports securely across the framework.
Inside `src/App.pysx`, you can universally load components deployed anywhere adjacent within the `src/` tree just by declaring their standard Python import identifier.

```python
# Automatically loads `/src/components/CounterApp.pysx` during AST Linker Build
import CounterApp

def App():
    return <main>
       <CounterApp />
    </main>
```

### Import Resolution

- `import ComponentName` → imports `./ComponentName.js`
- Components must be in the same directory or subdirectory
- ES6 module exports are generated automatically

---

## 10. Error Codes

ReactXPy provides detailed error messages with codes:

| Code | Type | Description |
|------|------|-------------|
| RX100 | SyntaxError | Unterminated string literal |
| RX101 | SyntaxError | Invalid character (e.g., `@`, `$`) |
| RX200 | SyntaxError | Expected identifier after `import` |
| RX201 | SyntaxError | Expected function name after `def` |
| RX202 | SyntaxError | Unexpected closing tag |
| RX203 | SyntaxError | Incomplete assignment (missing RHS) |
| RX204 | SyntaxError | Mismatched closing tag |
| RX205 | SyntaxError | Unterminated JSX tag |
| RX206 | SyntaxError | Empty attribute value |
| RX207 | SyntaxError | Expected opening tag |
| RX208 | SyntaxError | Incomplete `if` statement |
| RX209 | SyntaxError | Empty lambda expression |

### Common Errors and Fixes

**RX203 - Incomplete Assignment:**
```python
# ❌ Wrong
x, setX = 

# ✅ Correct
x, setX = useState(0)
```

**RX206 - Empty Attribute:**
```python
# ❌ Wrong
<input value= />

# ✅ Correct
<input value={value} />
```

**RX208 - Incomplete If:**
```python
# ❌ Wrong
if:
    pass

# ✅ Correct
if condition:
    pass
```

**RX209 - Empty Lambda:**
```python
# ❌ Wrong
<button onClick={lambda:}>

# ✅ Correct
<button onClick={lambda: handleClick()}>
```

---

## 11. Known Edge Cases & Limitations

As ReactXPy evolves, you must follow strict semantic logic to prevent Compiler Aborts:

1. **Nested Curly Braces**: Do not nest double-curly syntax logic mapping (e.g. `{{ foo: bar }}`). Render nested properties natively: `{ JSON.stringify({foo: "bar"}) }`.

2. **Lambda Colon Spacing**: Ensure `lambda:` declarations do not collide wildly with CSS ternary selectors formatting. Always apply safe white space boundaries inline.

3. **Empty Tag Void Checks**: Ensure JSX elements always close. Single tags like `<input>` and `<img>` must either strictly have closing boundaries `/>` or the HTML engine may drop nested trees recursively.

4. **Tuple Unpacking**: Only use tuple unpacking for `useState()`. Other Python tuple unpacking patterns may not work correctly.

5. **Python Built-ins**: Only use JavaScript-compatible built-ins. Python-specific functions like `len()`, `range()` are not automatically converted.

6. **Lambda Colon Spacing**: Ensure `lambda:` declarations have proper spacing. Avoid collisions with CSS ternary selectors.

7. **Empty Tag Void Checks**: Ensure JSX elements always close. Single tags like `<input>` and `<img>` must have closing boundaries `/>`.

---

## Appendix: Complete Example

```python
import Header
import Footer

def App():
    # State management
    count, setCount = useState(0)
    items, setItems = useState(["Apple", "Banana", "Cherry"])
    
    # Side effects
    useEffect(lambda: document.title = f"Count: {count}", [count])
    
    # Event handlers
    def addItem():
        setItems(items.concat([f"Item {items.length + 1}"]))
    
    # Render
    return <div class="app">
        <Header title="My App" />
        
        <main>
            <h1>Counter: {count}</h1>
            <button onClick={lambda: setCount(count + 1)}>
                Increment
            </button>
            
            <h2>Items</h2>
            <ul>
                {items.map(lambda item, i: 
                    <li key={i}>{item}</li>
                )}
            </ul>
            <button onClick={addItem}>Add Item</button>
            
            {count > 5 && <p>You've clicked more than 5 times!</p>}
        </main>
        
        <Footer />
    </div>
```

---


