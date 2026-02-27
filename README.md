# ReactXPy

**Write React components using Python syntax. No Node.js required.**

ReactXPy is a tool that lets you build modern web applications using Python syntax instead of JavaScript. If you know Python and want to create interactive websites with React, this tool is for you.

[![Version](https://img.shields.io/badge/version-0.3.2-blue.svg)](https://pypi.org/project/reactxpy/)
[![Python](https://img.shields.io/badge/python-3.7+-green.svg)](https://www.python.org/)
[![License](https://img.shields.io/badge/license-MIT-yellow.svg)](LICENSE)

---

## What is ReactXPy?

ReactXPy is a **transpiler** - a tool that converts code from one language to another. Specifically:

- **You write**: Python syntax with HTML-like tags (`.pysx` files)
- **ReactXPy converts**: Your code into JavaScript that runs in the browser
- **Result**: A working React application

### Why Use ReactXPy?

**If you love Python but need to build web apps, ReactXPy helps you:**

- ✅ Use Python's clean, indentation-based syntax
- ✅ Avoid complex JavaScript build tools (webpack, babel, etc.)
- ✅ Write React components without learning JavaScript first
- ✅ Get started in minutes, not hours
- ✅ Work offline after initial setup

**Example**: Instead of writing complex JavaScript:
```javascript
function Counter() {
  const [count, setCount] = React.useState(0);
  return React.createElement('button', {
    onClick: () => setCount(count + 1)
  }, count);
}
```

You write clean Python:
```python
def Counter():
    count, setCount = useState(0)
    return <button onClick={lambda: setCount(count + 1)}>{count}</button>
```

---

## Installation

### Step 1: Install Python
Make sure you have Python 3.7 or higher installed:
```bash
python3 --version
```

### Step 2: Install ReactXPy
Open your terminal and run:

```bash
pip install reactxpy
```

This downloads ReactXPy and automatically compiles the C++ compiler on your machine.

### Step 3: Verify Installation
Check that ReactXPy is installed correctly:

```bash
reactxpy --version
```

You should see: `reactxpy version 0.3.2`

### What Gets Installed?

When you install ReactXPy, you get three command-line tools:

| Command | What It Does |
|---------|--------------|
| `reactxpy` | Compiles individual `.pysx` files to JavaScript |
| `create-reactxpy-app` | Creates a new project with all necessary files |
| `build.py` / `dev.py` | Scripts in your project for building and development |

---

## Quick Start (5 Minutes)

### 1. Create a New Project

```bash
create-reactxpy-app my-app
```

This creates a folder called `my-app` with all the files you need.

### 2. Navigate to Your Project

```bash
cd my-app
```

### 3. Start the Development Server

```bash
python3 dev.py
```

### 4. Open Your Browser

Go to: `http://localhost:3000`

You should see your app running! The server automatically reloads when you save changes.

### What's Happening?

1. `create-reactxpy-app` sets up a complete project structure
2. `dev.py` watches your files and recompiles automatically
3. Your browser shows the live result instantly
4. Edit files in the `src/` folder and see changes immediately

---

## Learning ReactXPy Syntax

ReactXPy files use the `.pysx` extension. They look like Python but let you write HTML-like tags directly in your code.

### 1. Creating Components

A **component** is a reusable piece of your user interface. Think of it like a function that returns what should appear on screen.

```python
# Greeting.pysx
def Greeting(name):
    return <div class="card">
        <h2>Hello, {name}!</h2>
        <p>Welcome to ReactXPy</p>
    </div>
```

**Key Points:**
- Use `def` to define a component (just like Python functions)
- Return HTML-like tags using `<` and `>`
- Use `{curly braces}` to insert Python variables or expressions
- Indentation matters (standard Python rules)

### 2. Managing State (Remembering Data)

**State** is data that changes over time. For example: a counter value, form input, or toggle switch.

```python
# Counter.pysx
def Counter():
    # Create state: count starts at 0, setCount updates it
    count, setCount = useState(0)
    
    return <div>
        <p>You clicked {count} times</p>
        <button onClick={lambda: setCount(count + 1)}>
            Click me
        </button>
    </div>
```

**How it works:**
- `useState(0)` creates a variable `count` starting at 0
- `setCount` is a function to update `count`
- When you call `setCount`, the component re-renders with the new value
- `lambda:` creates a small function (like `def` but inline)

### 3. Handling Side Effects (useEffect)

**Side effects** are things that happen outside your component: API calls, modifying the document title, timers, etc.

```python
def DocumentTitle():
    title, setTitle = useState("My App")
    
    # This runs when 'title' changes
    useEffect(lambda: document.title = title, [title])
    
    return <input 
        value={title} 
        onChange={lambda e: setTitle(e.target.value)} 
    />
```

**How it works:**
- `useEffect` takes two arguments: a function and a list of dependencies
- The function runs after the component renders
- If dependencies change, the effect runs again
- Empty list `[]` means "run once when component loads"

### 4. Conditional Rendering (Showing/Hiding)

Show different content based on conditions:

```python
def Dashboard(user):
    return <div>
        {/* Show admin panel only if user is admin */}
        {user.isAdmin && <AdminPanel />}
        
        {/* Show different view based on status */}
        {user.status === "active" 
            ? <ActiveView /> 
            : <InactiveView />}
    </div>
```

**Two approaches:**
- `condition && <Component />` - Shows component only if condition is true
- `condition ? <IfTrue /> : <IfFalse />` - Shows one or the other

### 5. Rendering Lists

Display arrays of data:

```python
def TodoList():
    todos = [
        {id: 1, text: "Learn ReactXPy"},
        {id: 2, text: "Build something awesome"},
        {id: 3, text: "Share with friends"}
    ]
    
    return <ul>
        {todos.map(lambda todo: 
            <li key={todo.id}>{todo.text}</li>
        )}
    </ul>
```

**Key Points:**
- `map()` transforms each item in a list
- Always provide a unique `key` prop for list items
- `key` helps React track which items changed

---

## Project Structure

```
my-app/
├── src/
│   ├── components/      # Reusable components
│   └── App.pysx        # Main application
├── public/
│   ├── index.html      # HTML entry point
│   └── style.css       # Styles
├── runtime/
│   ├── react.development.js      # React library
│   ├── react-dom.development.js  # ReactDOM library
│   └── runtime.js      # ReactXPy runtime
├── dist/               # Compiled JavaScript
├── build.py            # Build script
└── dev.py              # Development server
```

---

## Troubleshooting & Error Messages

ReactXPy shows helpful error messages when something goes wrong. Each error includes:
- The filename and line number
- A visual pointer to the problem
- An error code (like RX203)
- A clear description of what went wrong

### Example Error

```
File "src/App.pysx", line 4

        x = 
       ^

RX203: SyntaxError: Incomplete assignment - missing right-hand side expression
```

**What this means:** You wrote `x = ` but didn't provide a value after the equals sign.

### Common Error Codes

| Code | What It Means | How to Fix |
|------|---------------|------------|
| RX100 | String not closed | Add closing quote `"` or `'` |
| RX101 | Invalid character | Remove or replace the strange character |
| RX200 | Import statement incomplete | Add the module name after `import` |
| RX201 | Function name missing | Add a name after `def` |
| RX203 | Assignment incomplete | Add a value after `=` |
| RX206 | HTML attribute empty | Add a value after `attribute=` |
| RX208 | If statement incomplete | Add a condition after `if` |
| RX209 | Lambda is empty | Add code after `lambda:` |

### Tips for Debugging

1. **Read the error message carefully** - It tells you exactly where the problem is
2. **Check line numbers** - The error shows which line has the issue
3. **Look at the `^` pointer** - It shows the exact position of the error
4. **Start simple** - If stuck, try removing code until it works, then add back slowly

---

## Commands

### `reactxpy`

Compile individual `.pysx` files:

```bash
reactxpy src/App.pysx -o dist/App.js
```

### `create-reactxpy-app`

Scaffold a new project:

```bash
create-reactxpy-app my-project
```

### `build.py`

Compile all source files:

```bash
python3 build.py
```

### `dev.py`

Start development server with hot reload:

```bash
python3 dev.py
```

---

## How ReactXPy Works (For the Curious)

Behind the scenes, ReactXPy does several things to turn your Python code into a working website:

### The Components

1. **C++ Compiler** (The Engine)
   - Reads your `.pysx` files
   - Understands both Python syntax and HTML-like tags
   - Converts everything to JavaScript
   - Runs extremely fast because it's written in C++

2. **Python Tools** (The Helpers)
   - `create-reactxpy-app`: Sets up new projects
   - `dev.py`: Watches your files and rebuilds automatically
   - `build.py`: Creates production-ready files

3. **Runtime** (The Bridge)
   - A small JavaScript file that connects your code to React
   - Handles Python-style hooks like `useState` and `useEffect`

### The Compilation Process

When you save a `.pysx` file, here's what happens:

```
Your Code (.pysx)
       ↓
[Lexer] - Breaks code into tokens
       ↓
[Parser] - Builds a tree structure (AST)
       ↓
[Generator] - Creates JavaScript code
       ↓
[Linker] - Combines all files
       ↓
Working Website!
```

**Example Transformation:**

Your Python code:
```python
def App():
    return <h1>Hello World</h1>
```

Becomes JavaScript:
```javascript
function App(props){
    return React.createElement("h1", null, "Hello World")
}
export default App;
```

---

## Browser Compatibility

ReactXPy generates standard ES6 JavaScript compatible with:

- Chrome 60+
- Firefox 55+
- Safari 12+
- Edge 79+

---

## Roadmap

- [x] JSX transpilation to React.createElement
- [x] useState and useEffect hooks
- [x] Hot reload development server
- [x] Error reporting with codes
- [x] Offline React files
- [ ] List comprehension syntax
- [ ] Async/await support
- [ ] Type hints integration

---

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

---

## License

MIT License - See [LICENSE](LICENSE) for details.

---

## Support

For questions or issues:
- GitHub Issues: [https://github.com/Anishkumar1928/reactxpy](https://github.com/Anishkumar1928/reactxpy)
- Documentation: [docs/](docs/)

---

**Built with ❤️ for Python and React developer @Anishkumar1928 **
