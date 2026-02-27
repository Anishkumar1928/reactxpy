import os
import subprocess

TESTS = [
    # --- LEXER LEVEL ---
    {
        "name": "invalid_char",
        "code": "def App():\n    return 100 @ 20",
        "expected_error": "SyntaxError",
        "expected_code": "RX101"
    },
    {
        "name": "unterminated_string",
        "code": "def App():\n    let msg = \"Hello World\n    return msg",
        "expected_error": "SyntaxError",
        "expected_code": "RX100"
    },

    # --- STRUCTURE LEVEL ---
    {
        "name": "missing_def_identifier",
        "code": "def (): \n    return 42",
        "expected_error": "Expected function name",
        "expected_code": "RX201"
    },
    {
        "name": "missing_import_identifier",
        "code": "import \n\ndef App(): pass",
        "expected_error": "Expected identifier after import",
        "expected_code": "RX200"
    },

    # --- JSX / EXPRESSION LEVEL ---
    {
        "name": "missing_open_tag",
        "code": "def App():\n    return </div>",
        "expected_error": "Unexpected closing tag",
        "expected_code": "RX202"
    },
    {
        "name": "unterminated_jsx_tag",
        "code": "def App():\n    return <div class=\"card\">\n      <span\n    </div>",
        "expected_error": "JSX parse failed/unterminated tag",
        "expected_code": "RX205"
    },
    {
        "name": "unmatched_jsx_close_tag",
        "code": "def App():\n    return <div>\n      <span>Hello</span>\n    </p>\n</div>",
        "expected_error": "Expected closing tag",
        "expected_code": "RX204"
    },
    
    # --- NEW ERROR VALIDATIONS ---
    {
        "name": "incomplete_assignment",
        "code": "def App():\n    inputValue, setInputValue = \n    return <div>Test</div>",
        "expected_error": "Incomplete assignment",
        "expected_code": "RX203"
    },
    {
        "name": "incomplete_assignment_simple",
        "code": "def App():\n    x = \n    return <div>Test</div>",
        "expected_error": "Incomplete assignment",
        "expected_code": "RX203"
    },
    {
        "name": "empty_jsx_attribute_value",
        "code": "def App():\n    return <input type=\"text\" value= />",
        "expected_error": "Empty attribute value",
        "expected_code": "RX206"
    },
    {
        "name": "empty_jsx_attribute_value_onchange",
        "code": "def App():\n    return <input onChange= />",
        "expected_error": "Empty attribute value",
        "expected_code": "RX206"
    },
    {
        "name": "incomplete_if_statement_colon",
        "code": "def App():\n    if:\n    return <div>Test</div>",
        "expected_error": "Incomplete 'if' statement",
        "expected_code": "RX208"
    },
    {
        "name": "incomplete_if_statement_newline",
        "code": "def App():\n    if \n    return <div>Test</div>",
        "expected_error": "Incomplete 'if' statement",
        "expected_code": "RX208"
    },
    {
        "name": "empty_lambda_function_call",
        "code": "def App():\n    useEffect(lambda:)\n    return <div>Test</div>",
        "expected_error": "Empty lambda expression",
        "expected_code": "RX209"
    },
    {
        "name": "empty_lambda_jsx_attribute",
        "code": "def App():\n    return <button onClick={lambda:}>Click</button>",
        "expected_error": "Empty lambda expression",
        "expected_code": "RX209"
    },
    {
        "name": "empty_lambda_with_space",
        "code": "def App():\n    useEffect(lambda: )\n    return <div>Test</div>",
        "expected_error": "Empty lambda expression",
        "expected_code": "RX209"
    },
    
    # --- VALID LEVEL (KITCHEN SINK) ---
    {
        "name": "valid_kitchen_sink",
        "code": """import React from "react"
import { useState } from "react"

def App(props):
    let [count, setCount] = useState(0)
    let isActive = true
    
    def handleClick(e):
        setCount(count + 1)
        
    if count > 5:
        isActive = false
        
    for i in range(3):
        console.log(i)
        
    return <div className="container" onClick={handleClick}>
        <h1>{props.title}</h1>
        {isActive ? <span>Active</span> : <span>Inactive</span>}
        <ul>
            <li>Item 1</li>
            <li>Item 2</li>
        </ul>
    </div>
""",
        "expected_error": None,
        "expected_code": None
    }
]

def run_tests():
    compiler_path = "./compiler/reactxpy_compiler"
    if not os.path.exists(compiler_path):
        print("Compiler not found. Run 'make -C compiler' first.")
        return

    os.makedirs("tests/error_tests", exist_ok=True)
    
    results = {
        "passed": [],
        "failed": [],
        "total": len(TESTS)
    }
    
    with open("error_report.txt", "w") as report:
        report.write("=" * 70 + "\n")
        report.write("ReactXPy Syntax Error Tests - Report\n")
        report.write("=" * 70 + "\n\n")

        for idx, test in enumerate(TESTS):
            filename = f"tests/error_tests/test_{test['name']}.pysx"
            with open(filename, "w") as f:
                f.write(test["code"])

            result = subprocess.run([compiler_path, filename], capture_output=True, text=True)
            output = result.stdout + result.stderr
            
            # Validate result
            expected_error = test.get("expected_error")
            expected_code = test.get("expected_code")
            
            test_result = {
                "name": test["name"],
                "expected_error": expected_error,
                "expected_code": expected_code,
                "actual_output": output,
                "passed": False
            }
            
            if expected_error is None:
                # Valid code should compile without errors
                # Check for actual error messages (not just "error" in path)
                has_error = "syntaxerror" in output.lower() or "rx" in output.lower() and "error" in output.lower()
                if has_error:
                    test_result["reason"] = "Valid code produced errors"
                    results["failed"].append(test_result)
                else:
                    test_result["passed"] = True
                    results["passed"].append(test_result)
            else:
                # Error code should produce expected error
                error_found = expected_error in output
                code_found = expected_code in output if expected_code else True
                
                if error_found and code_found:
                    test_result["passed"] = True
                    results["passed"].append(test_result)
                else:
                    if not error_found:
                        test_result["reason"] = f"Expected error '{expected_error}' not found"
                    elif not code_found:
                        test_result["reason"] = f"Expected code '{expected_code}' not found"
                    results["failed"].append(test_result)
            
            # Write to report
            status = "✅ PASS" if test_result["passed"] else "❌ FAIL"
            report.write(f"--- Test {idx+1}: {test['name']} {status} ---\n")
            report.write(f"Expected Error: {expected_error or 'None (valid code)'}\n")
            report.write(f"Expected Code: {expected_code or 'N/A'}\n")
            report.write(f"Code:\n{test['code']}\n\nOutput:\n{output}\n")
            if not test_result["passed"] and "reason" in test_result:
                report.write(f"FAILURE REASON: {test_result['reason']}\n")
            report.write("=" * 70 + "\n\n")
        
        # Summary
        report.write("\n" + "=" * 70 + "\n")
        report.write("SUMMARY\n")
        report.write("=" * 70 + "\n")
        report.write(f"Total Tests: {results['total']}\n")
        report.write(f"Passed: {len(results['passed'])}\n")
        report.write(f"Failed: {len(results['failed'])}\n")
        report.write(f"Success Rate: {len(results['passed'])/results['total']*100:.1f}%\n\n")
        
        if results["failed"]:
            report.write("Failed Tests:\n")
            for ft in results["failed"]:
                report.write(f"  - {ft['name']}: {ft.get('reason', 'Unknown')}\n")
    
    # Console output
    print(f"\n{'='*70}")
    print("ReactXPy Error Test Results")
    print(f"{'='*70}")
    print(f"Total Tests: {results['total']}")
    print(f"✅ Passed: {len(results['passed'])}")
    print(f"❌ Failed: {len(results['failed'])}")
    print(f"Success Rate: {len(results['passed'])/results['total']*100:.1f}%")
    
    if results["failed"]:
        print(f"\nFailed Tests:")
        for ft in results["failed"]:
            print(f"  - {ft['name']}: {ft.get('reason', 'Unknown')}")
    
    print(f"\n📄 Detailed report saved to: error_report.txt")

if __name__ == "__main__":
    run_tests()
