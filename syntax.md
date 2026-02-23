# ReactXPy Syntax Guide

ReactXPy is a syntax paradigm that allows you to write interactive frontend applications using native Python `.py` syntax coupled intricately with HTML (`JSX`).

Under the hood, ReactXPy bypasses Node.js entirely and utilizes a lightning-fast native transpiler to convert your code into ES6 JavaScript mapped directly to React DOM components.

Because ReactXPy transforms code structure rather than interpreting Python live in the browser, there are specific syntactical boundaries you must follow.

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

## 7. Zero-Dependency Toolchain Architecture

ReactXPy completely fundamentally eliminates Node.js and the NPM ecosystem from your build pipeline. It achieves this via a three-part native architecture:

### 1. `reactxpy_compiler` (Native Executable)

At the heart of the framework is a C++ executable. When you `pip install`, Python triggers GCC/Clang to compile the transpiler onto your specific OS (Windows `.exe` or Unix binary). It executes via the `reactxpy` CLI.
It reads `.pysx` files, skips the slow Python runtime, parses the syntax into a highly optimized AST memory tree, and drops ES6 Javascript bundles directly onto the disk in milliseconds.

### 2. The Lexer & Generator (AST Engine)

- **Lexer**: Analyzes tokens within the file. It detects when you type `def` and toggles on "Pythonic Scope". When it sees `<div`, it toggles the AST into "HTML / JSX Scope" safely isolating operators like `||` and `{}`.
- **Parser**: Converts the tokens into a component tree `ProgramNode -> FunctionNode -> JSXNode`.
- **Generator**: Scans the node tree. This is where `transformPythonKeywords` maps `True` -> `true` and the system injects `ReactXPy.useState()` namespace wrappers to safely bridge memory.
- **Linker**: Concatenates multiple `.js` component files into a single, cohesive `bundle.js` without requiring Webpack.

### 3. The Local Reloader (`dev.py` & `build.py`)

ReactXPy ships with a pure-Python threaded watcher server.

1. `build.py` runs recursively through `/src`, farming `.pysx` files into the C++ compiler.
2. `dev.py` starts a concurrent `http.server` running on `localhost:3000`.
3. It recursively watches `os.stat().st_mtime` for file modifications. When you hit save, the C++ executable fires, rebuilding the entire bundle instantly.
4. The backend hits the `/version` endpoint, which increments and triggers the frontend `window.location.reload()` hook.

---

## 8. State Hook Scope Caching

ReactXPy binds React's virtual state mapping to its `runtime.js` hook bridge. The `useState` variable destructuring scope enforces closure rules during transpilation.

```python
def Tracker():
    # count is locked into the closure of the Tracker function scope natively
    count, setCount = useState(0)

    def logAndIncrement():
        console.log("Current Hits: ", count)
        setCount(count + 1)

    return <button onClick={logAndIncrement}>Click</button>
```

---

## 9. Array Mapping Syntactical Structures

When rendering lists dynamically inside ReactXPy using JSX escape tags `{ ... }`, the compiler evaluates the mapping natively via Javascript `.map()` arrays.

**Rule:** Because the native Lexer handles one-pass translation, when inside `.map()`, you must currently use inline native `React.createElement` syntax instead of writing `<div />` strings if nesting deep elements. This safely prevents recursive AST looping violations.

```python
# TaskList.reactxpy
def TaskList(props):
    todos, setTodos = useState([
        {id: 1, text: "Finish ReactXPy", done: False},
        {id: 2, text: "Publish PyPI", done: True}
    ])

    # ❌ INCORRECT (Lexer limit inside nested mapping blocks)
    # {todos.map(lambda t: <div>{t.text}</div>)}

    # ✅ CORRECT ARRAY EVALUATION
    return <div class="task-list">
        {
          todos.map(lambda t: React.createElement(
              "div",
              { className: "item" },
              React.createElement("input", { type: "checkbox", checked: t.done }),
              React.createElement("span", null, t.text)
          ))
        }
    </div>
```

---

## 10. Cross-Module Importing (`App.reactxpy`)

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

---

## 11. Known Edge Cases & Limitations

As ReactXPy evolves, you must follow strict semantic logic to prevent Compiler Aborts:

1. **Nested Curly Braces**: Do not nest double-curly syntax logic mapping (e.g. `{{ foo: bar }}`). Render nested properties natively: `{ JSON.stringify({foo: "bar"}) }`.
2. **Lambda Colon Spacing**: Ensure `lambda:` declarations do not collide wildly with CSS ternary selectors formatting. Always apply safe white space boundaries inline.
3. **Empty Tag Void Checks**: Ensure JSX elements always close. Single tags like `<input>` and `<img>` must either strictly have closing boundaries `/>` or the HTML engine may drop nested trees recursively.
