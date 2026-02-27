# ReactXPy Language Specification

**Version:** 0.3.0  
**Status:** Active Development  
**Last Updated:** February 2026

---

## 1. Introduction

ReactXPy (Python Syntax for React) is a domain-specific language that combines Python's clean syntax with React's component model. This specification defines the syntax, semantics, and compilation behavior of `.pysx` files.

### 1.1 Design Goals

- **Familiarity**: Use Python syntax for JavaScript/React development
- **Zero Dependencies**: No Node.js, npm, or webpack required
- **Performance**: Native C++ compilation for fast build times
- **Interoperability**: Seamless integration with existing React ecosystems

### 1.2 File Extension

ReactXPy source files use the `.pysx` extension.

---

## 2. Lexical Structure

### 2.1 Source Encoding

Source files must be UTF-8 encoded.

### 2.2 Whitespace and Indentation

ReactXPy uses Python-style significant whitespace:

- **Spaces**: Preferred for indentation (4 spaces = 1 level)
- **Tabs**: Converted to 4 spaces
- **Indentation**: Defines block scope (functions, conditionals, loops)
- **Blank Lines**: Ignored for indentation logic

```python
def Component():          # Level 0
    x = 1                  # Level 1 (indented)
    if x > 0:              # Level 1
        return <div />     # Level 2
```

### 2.3 Comments

Only Python-style comments are supported:

```python
# This is a single-line comment

def Component():
    x = 1  # Inline comment
    return <div />
```

### 2.4 Keywords

Reserved keywords (case-sensitive):

| Keyword | Usage |
|---------|-------|
| `def` | Function/component definition |
| `return` | Return statement |
| `import` | Module import |
| `from` | Import source (optional) |
| `if` / `elif` / `else` | Conditional statements |
| `for` / `while` | Loops |
| `pass` | No-op statement |
| `lambda` | Anonymous functions |
| `and` / `or` / `not` | Logical operators |
| `True` / `False` / `None` | Boolean and null literals |

### 2.5 Identifiers

- Must start with a letter or underscore
- Can contain letters, digits, and underscores
- Case-sensitive
- Component names MUST start with uppercase

```python
# Valid identifiers
myVariable
_my_private
ComponentName
useState

# Invalid identifiers
123abc      # Starts with digit
my-variable # Contains hyphen
```

### 2.6 Literals

#### 2.6.1 String Literals

```python
# Single quotes
'Hello World'

# Double quotes
"Hello World"

# Triple quotes (multi-line)
"""This is a
multi-line string"""

# Template literals (backticks)
`Hello ${name}`
```

#### 2.6.2 Numeric Literals

```python
# Integers
42
-10
0

# Floats
3.14
-0.5

# Scientific notation
1e10
2.5e-3
```

#### 2.6.3 Boolean and Null Literals

```python
True      # Maps to JavaScript true
False     # Maps to JavaScript false
None      # Maps to JavaScript null
```

#### 2.6.4 Array Literals

```python
[1, 2, 3]
["a", "b", "c"]
[]  # Empty array
```

#### 2.6.5 Object Literals

```python
{name: "John", age: 30}
{active: True, count: 0}
{}  # Empty object
```

---

## 3. Components

### 3.1 Component Definition

Components are defined using Python function syntax:

```python
def ComponentName(props):
    # Component body
    return <jsx-element />
```

**Rules:**
- Component names MUST start with uppercase letter
- Props are received as function parameters
- Must return a JSX element or None

### 3.2 Props

Props are passed as positional parameters:

```python
def UserCard(name, email, avatar):
    return <div>
        <img src={avatar} />
        <h3>{name}</h3>
        <p>{email}</p>
    </div>

# Usage
<UserCard name="John" email="john@example.com" avatar="pic.jpg" />
```

### 3.3 Default Props

Default values are set using variable assignment:

```python
def Button(label, variant="primary"):
    return <button class={variant}>{label}</button>
```

---

## 4. JSX (JavaScript XML)

### 4.1 JSX Elements

JSX provides HTML-like syntax within Python:

```python
# HTML tags (lowercase)
<div>Content</div>
<span>Text</span>

# Components (uppercase)
<MyComponent />
<UserCard name="John" />
```

### 4.2 Self-Closing Tags

```python
<input type="text" />
<img src="photo.jpg" />
<br />
<hr />
```

### 4.3 JSX Attributes

#### 4.3.1 String Attributes

```python
<div class="container" id="main">
<a href="https://example.com" target="_blank">
```

#### 4.3.2 Expression Attributes

```python
# Boolean
<button disabled={isLoading}>

# Variable
<img src={imageUrl}>

# Expression
<div class={isActive ? "active" : "inactive"}>
```

#### 4.3.3 Special Attribute: Class

The `class` attribute is automatically converted to `className`:

```python
# ReactXPy
<div class="my-class">

# Compiles to JavaScript
React.createElement("div", { className: "my-class" })
```

#### 4.3.4 Style Attribute

Style accepts a JavaScript object:

```python
<div style={{color: "red", fontSize: 16}}>
```

### 4.4 JSX Children

#### 4.4.1 Text Children

```python
<div>Hello World</div>
```

#### 4.4.2 Expression Children

```python
<div>{user.name}</div>
<div>{count + 1}</div>
```

#### 4.4.3 Nested Elements

```python
<div>
    <header>
        <h1>Title</h1>
    </header>
    <main>Content</main>
</div>
```

### 4.5 Fragment Shorthand

Adjacent elements must be wrapped:

```python
# ❌ Invalid - adjacent elements
def Invalid():
    return <div>A</div><div>B</div>

# ✅ Valid - wrapped in parent
def Valid():
    return <div>
        <div>A</div>
        <div>B</div>
    </div>
```

---

## 5. React Hooks

### 5.1 useState

State management using Python tuple unpacking:

```python
def Counter():
    count, setCount = useState(0)
    name, setName = useState("")
    items, setItems = useState([1, 2, 3])
    
    return <div>
        <p>Count: {count}</p>
        <button onClick={lambda: setCount(count + 1)}>
            Increment
        </button>
    </div>
```

**Syntax:**
```
<getter>, <setter> = useState(<initial_value>)
```

### 5.2 useEffect

Side effects with dependency tracking:

```python
def DocumentTitle():
    title, setTitle = useState("My App")
    
    # Run on every render
    useEffect(lambda: console.log("Rendered"))
    
    # Run only when title changes
    useEffect(lambda: document.title = title, [title])
    
    # Run once on mount (empty deps)
    useEffect(lambda: console.log("Mounted"), [])
    
    # Cleanup on unmount
    useEffect(lambda: lambda: console.log("Cleanup"))
    
    return <div />
```

**Syntax:**
```
useEffect(<callback>)
useEffect(<callback>, [<dependencies>])
```

---

## 6. Event Handling

### 6.1 Lambda Functions

Use Python lambdas for inline event handlers:

```python
# No arguments
<button onClick={lambda: handleClick()}>

# With event
<input onChange={lambda e: setValue(e.target.value)}>

# Multiple statements (not recommended)
<button onClick={lambda: (console.log("A"), console.log("B"))}>
```

### 6.2 Named Functions

For complex logic, define a nested function:

```python
def Form():
    data, setData = useState({})
    
    def handleSubmit(e):
        e.preventDefault()
        console.log(data)
        # More logic...
    
    return <form onSubmit={handleSubmit}>
        {/* form fields */}
    </form>
```

### 6.3 Common Events

| Event | Handler |
|-------|---------|
| onClick | Mouse click |
| onChange | Input value change |
| onSubmit | Form submission |
| onKeyDown | Key press |
| onMouseEnter | Mouse hover |
| onMouseLeave | Mouse exit |
| onFocus | Element focus |
| onBlur | Element blur |

---

## 7. Conditional Rendering

### 7.1 Logical AND

```python
{ isLoading && <Spinner /> }
{ items.length > 0 && <List items={items} /> }
```

### 7.2 Ternary Operator

```python
{ isLoggedIn ? <Dashboard /> : <Login /> }
{ status === "success" ? <Success /> : status === "error" ? <Error /> : <Loading /> }
```

### 7.3 If Statements

For complex conditions, use Python if:

```python
def Greeting(user):
    if user:
        if user.isAdmin:
            return <AdminGreeting name={user.name} />
        return <UserGreeting name={user.name} />
    return <GuestGreeting />
```

---

## 8. List Rendering

### 8.1 Basic Mapping

```python
def TodoList():
    todos = [
        {id: 1, text: "Learn ReactXPy"},
        {id: 2, text: "Build something"}
    ]
    
    return <ul>
        {todos.map(lambda todo: 
            <li key={todo.id}>{todo.text}</li>
        )}
    </ul>
```

### 8.2 With Index

```python
{items.map(lambda item, index: 
    <li key={index}>{index + 1}. {item}</li>
)}
```

### 8.3 Filtering

```python
{users
    .filter(lambda u: u.active)
    .map(lambda u: <User key={u.id} data={u} />)
}
```

### 8.4 Key Prop

Always provide a `key` prop when rendering lists:

```python
# ✅ Good - unique key
{items.map(lambda item => 
    <div key={item.id}>{item.name}</div>
)}

# ⚠️ Okay - index as key (only if no unique id)
{items.map(lambda item, index => 
    <div key={index}>{item.name}</div>
)}
```

---

## 9. Imports and Exports

### 9.1 Importing Components

```python
import Button
import Header
import Footer

def App():
    return <div>
        <Header />
        <main>Content</main>
        <Footer />
    </div>
```

### 9.2 Import Resolution

Imports are resolved relative to the source file:

```python
import Component        # ./Component.pysx
import utils.helper     # ./utils/helper.pysx
```

### 9.3 Exports

The last function in a file is automatically exported as default:

```python
import Button

def App():              # This is exported
    return <div>
        <Button />
    </div>

# Compiles to:
# export default App;
```

---

## 10. Control Flow

### 10.1 If/Elif/Else

```python
def Status(code):
    if code == 200:
        return <Success />
    elif code == 404:
        return <NotFound />
    elif code >= 500:
        return <ServerError />
    else:
        return <Unknown />
```

### 10.2 For Loops

```python
def Numbers():
    result = []
    for i in range(5):
        result.append(i)
    return <div>{result.join(", ")}</div>
```

### 10.3 While Loops

```python
def Counter(max):
    count = 0
    result = []
    while count < max:
        result.append(count)
        count += 1
    return <div>{result.join(", ")}</div>
```

---

## 11. Type Conversions

### 11.1 Python to JavaScript

| Python | JavaScript |
|--------|------------|
| `True` | `true` |
| `False` | `false` |
| `None` | `null` |
| `and` | `&&` |
| `or` | `\|\|` |
| `not` | `!` |
| `lambda` | `=>` (arrow function) |

### 11.2 Operators

JavaScript operators work inside expressions:

```python
# Arithmetic
{count + 1}
{total - discount}
{price * quantity}
{sum / count}

# Comparison
{age >= 18}
{name === "John"}
{value !== null}

# Logical
{isActive && hasPermission}
{isAdmin || isModerator}
{!isLoading}
```

---

## 12. Error Handling

### 12.1 Compilation Errors

ReactXPy provides detailed error messages:

```
File "App.pysx", line 5

    x, setX = 
      ^

RX203: SyntaxError: Incomplete assignment - missing right-hand side expression
```

### 12.2 Error Codes

See [ERROR_CODES.md](ERROR_CODES.md) for complete reference.

---

## 13. Best Practices

### 13.1 Component Structure

```python
import Header
import Footer

def Layout(children):
    return <div class="layout">
        <Header />
        <main class="content">{children}</main>
        <Footer />
    </div>
```

### 13.2 State Management

```python
def Form():
    # Group related state
    formData, setFormData = useState({
        name: "",
        email: "",
        message: ""
    })
    
    def updateField(field, value):
        setFormData({...formData, [field]: value})
    
    return <form>
        <input 
            value={formData.name} 
            onChange={lambda e: updateField("name", e.target.value)}
        />
    </form>
```

### 13.3 Event Handlers

```python
def TodoItem(todo, onToggle, onDelete):
    # Inline for simple cases
    return <div>
        <input 
            type="checkbox" 
            checked={todo.done}
            onChange={lambda: onToggle(todo.id)}
        />
        <button onClick={lambda: onDelete(todo.id)}>Delete</button>
    </div>
```

---

## 14. Grammar Reference

### 14.1 EBNF Grammar

```ebnf
program         ::= { import_stmt | function_def }

import_stmt     ::= "import" identifier [ "from" string ]

function_def    ::= "def" identifier "(" [ param_list ] ")" ":" block

param_list      ::= identifier { "," identifier }

block           ::= INDENT stmt+ DEDENT

stmt            ::= simple_stmt | compound_stmt

simple_stmt     ::= expr_stmt | return_stmt | pass_stmt

compound_stmt   ::= if_stmt | for_stmt | while_stmt | function_def

expr_stmt       ::= identifier "=" expression

return_stmt     ::= "return" [ expression ]

if_stmt         ::= "if" expression ":" block 
                    { "elif" expression ":" block } 
                    [ "else" ":" block ]

for_stmt        ::= "for" identifier "in" expression ":" block

while_stmt      ::= "while" expression ":" block

expression      ::= jsx_expr | logical_expr | lambda_expr

jsx_expr        ::= "<" identifier { attribute } [ "/>" | ">" { jsx_child } "</" identifier ">" ]

attribute       ::= identifier "=" ( string | "{" expression "}" )

jsx_child       ::= text | "{" expression "}" | jsx_expr

logical_expr    ::= comparison { ( "and" | "or" ) comparison }

comparison      ::= additive { ( "==" | "!=" | "<" | ">" | "<=" | ">=" ) additive }

additive        ::= multiplicative { ( "+" | "-" ) multiplicative }

multiplicative  ::= primary { ( "*" | "/" | "%" ) primary }

primary         ::= identifier | literal | "(" expression ")" | function_call

function_call   ::= identifier "(" [ arg_list ] ")"

arg_list        ::= expression { "," expression }

lambda_expr     ::= "lambda" [ param_list ] ":" expression

identifier      ::= letter { letter | digit | "_" }

literal         ::= string | number | boolean | "None" | array | object
```

---

## 15. Implementation Notes

### 15.1 Compiler Architecture

The ReactXPy compiler consists of:

1. **Lexer** - Tokenizes source code
2. **Parser** - Builds AST
3. **Generator** - Emits JavaScript
4. **Linker** - Bundles files

### 15.2 Runtime Requirements

Compiled ReactXPy code requires:
- React 16.8+ (for Hooks)
- Modern browser with ES6 support

### 15.3 Performance

- Compilation: ~1ms per 1000 lines
- Output: ES6 modules, no runtime overhead

---

## Appendix A: Complete Example

```python
import Header
import TodoItem

def TodoApp():
    # State
    todos, setTodos = useState([
        {id: 1, text: "Learn ReactXPy", done: false},
        {id: 2, text: "Build an app", done: false}
    ])
    newTodo, setNewTodo = useState("")
    
    # Effects
    useEffect(lambda: document.title = f"Todos ({todos.length})", [todos])
    
    # Handlers
    def addTodo():
        if not newTodo:
            return
        setTodos(todos.concat([{
            id: Date.now(),
            text: newTodo,
            done: false
        }]))
        setNewTodo("")
    
    def toggleTodo(id):
        setTodos(todos.map(lambda t: 
            t.id === id ? {...t, done: !t.done} : t
        ))
    
    def deleteTodo(id):
        setTodos(todos.filter(lambda t: t.id !== id))
    
    # Render
    return <div class="todo-app">
        <Header title="My Todos" />
        
        <div class="input-section">
            <input 
                type="text"
                value={newTodo}
                placeholder="Add a todo..."
                onChange={lambda e: setNewTodo(e.target.value)}
                onKeyDown={lambda e: e.key === "Enter" and addTodo()}
            />
            <button onClick={addTodo}>Add</button>
        </div>
        
        <div class="todo-list">
            {todos.length === 0 && <p class="empty">No todos yet!</p>}
            {todos.map(lambda todo =>
                <TodoItem 
                    key={todo.id}
                    todo={todo}
                    onToggle={toggleTodo}
                    onDelete={deleteTodo}
                />
            )}
        </div>
        
        <footer>
            <p>{todos.filter(lambda t: !t.done).length} items remaining</p>
        </footer>
    </div>
```

---

## Appendix B: Changelog

### v0.3.0 (Current)
- Added error validation (RX203, RX206, RX208, RX209)
- Improved JSX attribute handling
- Enhanced error messages with source context

### v0.2.0
- Added useState and useEffect hooks
- ES6 module output
- Python indentation support

### v0.1.0
- Initial release
- Basic component compilation
- JSX support
