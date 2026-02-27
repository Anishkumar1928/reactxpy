#!/usr/bin/env python3
"""
ReactXPy Valid Code Test Suite
Tests 50 normal programming scenarios to ensure valid code compiles correctly.
"""

import os
import subprocess

VALID_TESTS = [
    # === BASIC COMPONENTS (1-5) ===
    {"name": "simple_hello", "code": 'def App():\n    return <h1>Hello World</h1>'},
    {"name": "component_props", "code": 'def Greeting(name):\n    return <p>Hello {name}!</p>'},
    {"name": "multiple_props", "code": 'def Card(title, content):\n    return <div>\n        <h2>{title}</h2>\n        <p>{content}</p>\n    </div>'},
    {"name": "nested_elements", "code": 'def Box():\n    return <div>\n        <span>Inner</span>\n    </div>'},
    {"name": "self_closing", "code": 'def Form():\n    return <form>\n        <input type="text" />\n        <br />\n    </form>'},

    # === JSX ATTRIBUTES (6-10) ===
    {"name": "string_attribute", "code": 'def Link():\n    return <a href="https://example.com">Link</a>'},
    {"name": "class_attribute", "code": 'def Box():\n    return <div Class="container">Content</div>'},
    {"name": "id_attribute", "code": 'def Section():\n    return <section id="main">Content</section>'},
    {"name": "expression_attribute", "code": 'def Button(disabled):\n    return <button disabled={disabled}>Click</button>'},
    {"name": "multiple_attributes", "code": 'def Input():\n    return <input type="text" placeholder="Enter name" value="John" />'},

    # === USESTATE HOOK (11-15) ===
    {"name": "usestate_number", "code": 'def Counter():\n    count, setCount = useState(0)\n    return <p>{count}</p>'},
    {"name": "usestate_string", "code": 'def Input():\n    text, setText = useState("")\n    return <p>{text}</p>'},
    {"name": "usestate_boolean", "code": 'def Toggle():\n    on, setOn = useState(false)\n    return <p>{on ? "Yes" : "No"}</p>'},
    {"name": "usestate_array", "code": 'def List():\n    items, setItems = useState([1, 2, 3])\n    return <p>{items.length}</p>'},
    {"name": "usestate_object", "code": 'def User():\n    user, setUser = useState({name: "John"})\n    return <p>{user.name}</p>'},

    # === USEEFFECT HOOK (16-20) ===
    {"name": "useeffect_basic", "code": 'def App():\n    useEffect(lambda: console.log("mounted"))\n    return <div>App</div>'},
    {"name": "useeffect_deps", "code": 'def App():\n    count, setCount = useState(0)\n    useEffect(lambda: console.log(count), [count])\n    return <div>{count}</div>'},
    {"name": "useeffect_empty_deps", "code": 'def App():\n    useEffect(lambda: console.log("once"), [])\n    return <div>App</div>'},
    {"name": "useeffect_cleanup", "code": 'def App():\n    useEffect(lambda: lambda: console.log("cleanup"))\n    return <div>App</div>'},
    {"name": "useeffect_localstorage", "code": 'def App():\n    useEffect(lambda: localStorage.setItem("key", "value"))\n    return <div>App</div>'},

    # === EVENT HANDLERS (21-25) ===
    {"name": "onclick_handler", "code": 'def Button():\n    return <button onClick={lambda: alert("clicked")}>Click</button>'},
    {"name": "onchange_handler", "code": 'def Input():\n    return <input onChange={lambda e: console.log(e.target.value)} />'},
    {"name": "onsubmit_handler", "code": 'def Form():\n    return <form onSubmit={lambda e: e.preventDefault()}>Submit</form>'},
    {"name": "onmouseover_handler", "code": 'def Box():\n    return <div onMouseOver={lambda: console.log("over")}>Hover</div>'},
    {"name": "event_with_state", "code": 'def Counter():\n    count, setCount = useState(0)\n    return <button onClick={lambda: setCount(count + 1)}>{count}</button>'},

    # === CONDITIONAL RENDERING (26-30) ===
    {"name": "logical_and", "code": 'def Alert(show):\n    return <div>{show && <span>Warning</span>}</div>'},
    {"name": "ternary_operator", "code": 'def Status(active):\n    return <span>{active ? "Active" : "Inactive"}</span>'},
    {"name": "null_conditional", "code": 'def Maybe(data):\n    return <div>{data ? data.name : "No data"}</div>'},
    {"name": "nested_ternary", "code": 'def Grade(score):\n    return <span>{score > 90 ? "A" : score > 80 ? "B" : "C"}</span>'},
    {"name": "boolean_condition", "code": 'def Toggle(isOn):\n    return <div>{isOn && <p>Power is on</p>}</div>'},

    # === LIST RENDERING (31-35) ===
    {"name": "map_simple", "code": 'def List():\n    items = ["a", "b", "c"]\n    return <ul>{items.map(lambda x: <li>{x}</li>)}</ul>'},
    {"name": "map_with_key", "code": 'def Users():\n    users = [{id: 1, name: "John"}]\n    return <ul>{users.map(lambda u: <li key={u.id}>{u.name}</li>)}</ul>'},
    {"name": "map_index", "code": 'def Items():\n    items = ["a", "b"]\n    return <ul>{items.map(lambda x, i: <li key={i}>{x}</li>)}</ul>'},
    {"name": "filter_map", "code": 'def ActiveUsers():\n    users = [{active: true, name: "John"}]\n    return <ul>{users.filter(lambda u: u.active).map(lambda u: <li>{u.name}</li>)}</ul>'},
    {"name": "nested_map", "code": 'def Grid():\n    rows = [[1, 2], [3, 4]]\n    return <div>{rows.map(lambda row: <div>{row.map(lambda cell: <span>{cell}</span>)}</div>)}</div>'},

    # === IMPORTS (36-40) ===
    {"name": "import_single", "code": 'import Button\n\ndef App():\n    return <Button />'},
    {"name": "import_multiple", "code": 'import Header\nimport Footer\n\ndef App():\n    return <div><Header /><Footer /></div>'},
    {"name": "import_usage", "code": 'import Card\n\ndef App():\n    return <Card title="Hello" content="World" />'},
    {"name": "import_nested", "code": 'import Layout\n\ndef App():\n    return <Layout><p>Child</p></Layout>'},
    {"name": "import_sibling", "code": 'import Nav\nimport Sidebar\n\ndef App():\n    return <div><Nav /><Sidebar /></div>'},

    # === PYTHON FEATURES (41-45) ===
    {"name": "variable_assignment", "code": 'def App():\n    x = 10\n    y = 20\n    return <p>{x + y}</p>'},
    {"name": "string_interpolation", "code": 'def Greet(name):\n    message = f"Hello {name}"\n    return <p>{message}</p>'},
    {"name": "list_literal", "code": 'def App():\n    items = [1, 2, 3, 4, 5]\n    return <p>{items[0]}</p>'},
    {"name": "dict_literal", "code": 'def App():\n    user = {"name": "John", "age": 30}\n    return <p>{user.name}</p>'},
    {"name": "lambda_expression", "code": 'def App():\n    double = lambda x: x * 2\n    return <p>{double(5)}</p>'},

    # === COMPLEX SCENARIOS (46-50) ===
    {"name": "form_complete", "code": 'def Form():\n    email, setEmail = useState("")\n    password, setPassword = useState("")\n    return <form onSubmit={lambda e: e.preventDefault()}>\n        <input type="email" value={email} onChange={lambda e: setEmail(e.target.value)} />\n        <input type="password" value={password} onChange={lambda e: setPassword(e.target.value)} />\n        <button type="submit">Login</button>\n    </form>'},
    {"name": "todo_item", "code": 'def TodoItem(text, completed):\n    return <li Class={completed ? "done" : ""}>\n        <input type="checkbox" checked={completed} />\n        <span>{text}</span>\n        <button onClick={lambda: console.log("delete")}>Delete</button>\n    </li>'},
    {"name": "modal_dialog", "code": 'def Modal(isOpen, onClose):\n    return <div Class={isOpen ? "modal open" : "modal"}>\n        <div Class="modal-content">\n            <button onClick={onClose}>Close</button>\n        </div>\n    </div>'},
    {"name": "navbar", "code": 'import NavLink\n\ndef Navbar():\n    return <nav>\n        <NavLink href="/">Home</NavLink>\n        <NavLink href="/about">About</NavLink>\n    </nav>'},
    {"name": "counter_with_effect", "code": 'def Counter():\n    count, setCount = useState(0)\n    useEffect(lambda: document.title = f"Count: {count}", [count])\n    return <div>\n        <p>Count: {count}</p>\n        <button onClick={lambda: setCount(count + 1)}>+</button>\n        <button onClick={lambda: setCount(count - 1)}>-</button>\n    </div>'},
]


def run_tests():
    compiler_path = "./compiler/reactxpy_compiler"
    if not os.path.exists(compiler_path):
        print("❌ Compiler not found at", compiler_path)
        return

    os.makedirs("tests/valid_tests", exist_ok=True)
    
    results = {"passed": [], "failed": [], "total": len(VALID_TESTS)}
    
    with open("valid_tests_report.txt", "w") as report:
        report.write("=" * 80 + "\n")
        report.write("ReactXPy Valid Code Test Suite - Report\n")
        report.write("=" * 80 + "\n\n")
        report.write(f"Total Tests: {len(VALID_TESTS)}\n")
        report.write("Testing: Normal/valid code that programmers write\n\n")

        for idx, test in enumerate(VALID_TESTS, 1):
            filename = f"tests/valid_tests/test_{test['name']}.pysx"
            output_file = f"tests/valid_tests/test_{test['name']}.js"
            
            with open(filename, "w") as f:
                f.write(test["code"])

            result = subprocess.run(
                [compiler_path, filename, "-o", output_file],
                capture_output=True, text=True
            )
            output = result.stdout + result.stderr
            
            # Check if compilation succeeded
            success = "✅ Compiled" in output and result.returncode == 0
            
            test_result = {
                "num": idx,
                "name": test["name"],
                "passed": success,
                "output": output
            }
            
            if success:
                results["passed"].append(test_result)
                status = "✅ PASS"
            else:
                results["failed"].append(test_result)
                status = "❌ FAIL"
            
            # Write to report
            report.write(f"--- Test {idx}: {test['name']} {status} ---\n")
            report.write(f"Code:\n{test['code']}\n\n")
            report.write(f"Output:\n{output}\n")
            report.write("-" * 80 + "\n\n")
        
        # Summary
        report.write("\n" + "=" * 80 + "\n")
        report.write("SUMMARY\n")
        report.write("=" * 80 + "\n")
        report.write(f"Total Tests: {results['total']}\n")
        report.write(f"Passed: {len(results['passed'])}\n")
        report.write(f"Failed: {len(results['failed'])}\n")
        report.write(f"Success Rate: {len(results['passed'])/results['total']*100:.1f}%\n\n")
        
        if results["failed"]:
            report.write("Failed Tests:\n")
            for ft in results["failed"]:
                report.write(f"  - Test {ft['num']}: {ft['name']}\n")
    
    # Console output
    print("=" * 80)
    print("ReactXPy Valid Code Test Results")
    print("=" * 80)
    print(f"Total Tests: {results['total']}")
    print(f"✅ Passed: {len(results['passed'])}")
    print(f"❌ Failed: {len(results['failed'])}")
    print(f"Success Rate: {len(results['passed'])/results['total']*100:.1f}%")
    
    if results["failed"]:
        print(f"\nFailed Tests:")
        for ft in results["failed"]:
            print(f"  - Test {ft['num']}: {ft['name']}")
            print(f"    Output: {ft['output'][:100]}...")
    
    print(f"\n📄 Detailed report saved to: valid_tests_report.txt")
    print(f"📁 Test files saved to: tests/valid_tests/")


if __name__ == "__main__":
    run_tests()
