window.DOCS_DATA = {
    c1: `def Button(label):
    button_name = label + " Click Me!"
    return <button class="btn-primary">
        <span>{button_name}</span>
    </button>`,

    c2: `def Counter():
    count, setCount = useState(0)
    useEffect(lambda: document.title = "Count: " + count, [count])
    return <div class="counter-box">
        <b>Hits: {count}</b>
        <button onClick={lambda: setCount(count + 1)}>Increment</button>
    </div>`,

    c3a: `# CORRECT
<button onClick={lambda: console.log("clicked")}>`,

    c3b: `# Passes the 'event' (e) parameter downstream
<input onChange={lambda e: console.log(e.target.value)} />`,

    c3c: `def LoginScreen():
    username, setUsername = useState('')

    def handleSubmit():
        if (username == 'admin'):
            alert('Welcome!')
        else:
            alert('Denied.')

    # Reference the internal function pointer
    return <button onClick={handleSubmit}>Login</button>`,

    c4a: `{ isLoggedIn ? <h1>Welcome!</h1> : <h1>Sign In</h1> }`,
    
    c4b: `{ hasErrors && <div class="error-modal">Failed</div> }`,

    c5: `# Before Compilation (ReactXPy)
def FeatureFlag():
    isEnabled = True
    sessionObj = None

# After Compilation (dist/bundle.js)
# function FeatureFlag() {
#    let isEnabled = true;
#    let sessionObj = null;
# }`,

    c6: `# You write this:
<div class="card">

# The framework natively mounts this to the React DOM tree:
# React.createElement('div', { className: 'card' })`,

    c8: `def Tracker():
    # count is locked into the closure of the Tracker function scope natively
    count, setCount = useState(0)

    def logAndIncrement():
        console.log('Current Hits: ', count)
        setCount(count + 1)

    return <button onClick={logAndIncrement}>Click</button>`,

    c9: `{ todos.map(lambda t: React.createElement("div", {className: "item"}, t.text)) }`,

    c10: `# Automatically loads \`/src/components/CounterApp.pysx\` during AST Linker Build
import CounterApp

def App():
    return <main>
       <CounterApp />
    </main>`
};
