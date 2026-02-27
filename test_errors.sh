#!/bin/bash

echo "=== ReactXPy Error Handling Demo ==="
echo

# Test 1: Incomplete Assignment (RX203)
echo "Test 1: Incomplete Assignment"
echo "------------------------------"
cat > /tmp/test_error.pysx << 'EOF'
def App():
    x = 
    return <div>Test</div>
EOF

echo "Input:"
cat /tmp/test_error.pysx
echo
echo "Output:"
reactxpy /tmp/test_error.pysx -o /tmp/out.js 2>&1
echo

# Test 2: Empty Attribute (RX206)
echo "Test 2: Empty Attribute Value"
echo "------------------------------"
cat > /tmp/test_error.pysx << 'EOF'
def App():
    return <input value= />
EOF

echo "Input:"
cat /tmp/test_error.pysx
echo
echo "Output:"
reactxpy /tmp/test_error.pysx -o /tmp/out.js 2>&1
echo

# Test 3: Incomplete If Statement (RX208)
echo "Test 3: Incomplete If Statement"
echo "------------------------------"
cat > /tmp/test_error.pysx << 'EOF'
def App():
    if:
    return <div>Test</div>
EOF

echo "Input:"
cat /tmp/test_error.pysx
echo
echo "Output:"
reactxpy /tmp/test_error.pysx -o /tmp/out.js 2>&1
echo

# Test 4: Empty Lambda (RX209)
echo "Test 4: Empty Lambda Expression"
echo "------------------------------"
cat > /tmp/test_error.pysx << 'EOF'
def App():
    useEffect(lambda:)
    return <div>Test</div>
EOF

echo "Input:"
cat /tmp/test_error.pysx
echo
echo "Output:"
reactxpy /tmp/test_error.pysx -o /tmp/out.js 2>&1
echo

# Test 5: Valid Code (Should Compile)
echo "Test 5: Valid Code (Should Compile Successfully)"
echo "------------------------------"
cat > /tmp/test_valid.pysx << 'EOF'
def App():
    count, setCount = useState(0)
    return <button onClick={lambda: setCount(count + 1)}>{count}</button>
EOF

echo "Input:"
cat /tmp/test_valid.pysx
echo
echo "Output:"
reactxpy /tmp/test_valid.pysx -o /tmp/out.js 2>&1
echo

echo "=== Demo Complete ==="
