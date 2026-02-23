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

---

## 7. Limitations & Horizon Features

- **Array Mapping syntax**: Currently, inline map arrays require `React.createElement` wrappers if parsing complex nested tags, due to the single-pass lexing engine limitations:
  `{items.map(lambda i: React.createElement("li", null, i))}`
- **ES6 Imports**: Because the pipeline bundles your AST together via a lightweight zero-dependency wrapper loader, you do not need `import "components"` headers if everything is natively within the `/src/` folder scope! The transpiler handles the module maps globally for you.
