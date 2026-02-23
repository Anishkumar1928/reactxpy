# PYSX (Python Syntax for React)

PYSX is an innovative, lightning-fast compiler that lets you write beautiful, interactive web applications using clean and readable **Python syntax**, which natively compiles down directly into **React JavaScript** (`React.createElement`).

Stop fighting with Node.js configuration hell! With PYSX, you get the elegance of Python and the power of the React component ecosystem.

---

## 🚀 Features (v1)

- **Python Function Syntax:** Define components cleanly using `def ComponentName(props):`.
- **Built-in JSX:** Natively write HTML tags inside your Python functions.
- **Props & Interpolation:** Pass data seamlessly using `{props}` and `{expressions}`.
- **Native Event Handling:** Bind functions easily via `onClick={funcName}`.
- **Conditional Rendering:** Powerful inline booleans like `{flag && <div class="badge">Active</div>}`.
- **Zero-Config CLI Tooling:** Comes with a built-in project generator (`create-pysx-app`) and bundler. No Webpack, Vite, or Babel required!
- **Lightning Fast:** The transpiler is built entirely in C++ for maximum compilation speed.

---

## 📦 Installation

To install `pysx` globally on your system, you just need Python. Run:

```bash
pip install pysx
```

_(If developing locally, clone this repo and run `pip install -e .` from the root directory)._

Ensure your installation was successful by checking the version:

```bash
pysx --version
# Output: pysx version 0.1.0
```

---

## 🛠️ Quick Start (Create a Project)

PYSX comes with a brilliant interactive scaffolding tool modeled after modern frontend frameworks. To create your first app:

1. Run the interactive generator:
   ```bash
   create-pysx-app
   ```
2. Type your project name (e.g., `my-awesome-app`) when prompted!

3. Navigate into your new project, compile it, and open it:
   ```bash
   cd my-awesome-app
   ./build.sh
   open public/index.html
   ```

Boom! You just built a React application exclusively using Python syntax.

---

## 📖 PYSX Syntax Overview

### 1. Components & JSX

Components are defined just like Python functions using `def`. They return native JSX tags.

```python
# Greet.pysx
def Greet(name):
    return <div class="greeting-card">
        <h3>Hello {name}!</h3>
        <p>Welcome to PYSX.</p>
    </div>
```

### 2. Events and Logic

You can define standard logic functions and seamlessly attach them to UI events without weird binding issues context binding.

```python
# Counter.pysx
def triggerAlert():
    alert("Button was clicked!")

def Counter():
    return <div class="counter">
        <button onClick={triggerAlert}>Click Me</button>
    </div>
```

### 3. Conditional Rendering & Advanced Features

PYSX v1's engine natively supports complex conditional elements, automatically translating `class="..."` properties into React's `{className: "..."}`. CSS classes and variables will just work natively.

```python
# Status.pysx
def Status(isActive):
    return <div>
        <p>Current Status:</p>
        {isActive && <div class="badge-success">Online</div>}
    </div>
```

### 4. File Imports and Bundling

Writing a large app? Split it up! Use simple `import` statements at the top of your file.

```python
# App.pysx
import Greet
import Counter

def App():
    return <main class="app">
        <h1>Dashboard</h1>
        <Greet name="Developer" />
        <Counter />
    </main>
```

When you run the compiler (e.g., via `./build.sh`), PYSX will intelligently gather all components and emit a singular, deeply-linked, browser-safe `dist/bundle.js` script!

---

## ⚙️ How it Works

1.  **Lexical Analysis & Parsing:** The C++ compiler scans your `.pysx` files, understanding the unique blending of Python whitespace, colons, and HTML tags.
2.  **Transpilation:** The C++ generator translates the syntax tree into raw `React.createElement` syntax. HTML parameters (like `class`) are converted to their DOM property equivalents (like `className`), and Javascript reserved variables like `True`/`False` are corrected to boolean literals `true`/`false`.
3.  **No `export` Noise:** PYSX strips out ES module syntax entirely from the bundled artifact. This means you do absolutely **not** need a local live-server (like `http-server`) simply to run your Javascript files! You can literally double-click `index.html` on your Desktop and it renders instantly.

---

## 🔮 Roadmap (v2 & Beyond)

PYSX is actively developed. The following features are currently being worked on for future releases:

- [ ] Interactive state management (`useState` equivalent).
- [ ] Hooks (`useEffect`, lifecycle mapping).
- [ ] Support for inline array mapping (e.g., `<item for item in list>`).
- [ ] Asynchronous components.

---

### License

MIT License - Have fun building!
