import os
import sys

def create_file(path, content):
    with open(path, 'w') as f:
        f.write(content.strip() + '\n')
    print(f"Created {path}")

def main():
    if len(sys.argv) > 1 and sys.argv[1] in ["-h", "--help"]:
        print("Usage: create-pysx-app [project-name]")
        print("\\nOptions:")
        print("  -h, --help       Show this help message and exit.")
        print("  -v, --version    Show the version of create-pysx-app.")
        print("\\nExample:")
        print("  create-pysx-app my-new-project")
        sys.exit(0)

    if len(sys.argv) > 1 and sys.argv[1] in ["-v", "--version"]:
        print("create-pysx-app version 0.1.0")
        sys.exit(0)

    print("\\033[0;34mWelcome to create-pysx-app! Let's scaffold your new project.\\033[0m\\n")

    project_name = ""
    if len(sys.argv) > 1:
        project_name = sys.argv[1]
        
    while not project_name:
        try:
            project_name = input("? What is your project named? ")
        except (KeyboardInterrupt, EOFError):
            print("\\nOperation cancelled.")
            sys.exit(1)
            
    project_name = project_name.strip()
    if not project_name:
        print("Error: Project name cannot be empty.")
        sys.exit(1)
    
    if os.path.exists(project_name):
        print(f"Error: Directory '{project_name}' already exists.")
        sys.exit(1)
        
    print(f"\\n🚀 Creating a new PYSX project in {os.path.abspath(project_name)}...")
    
    # Create directory structure
    dirs = [
        project_name,
        os.path.join(project_name, "public"),
        os.path.join(project_name, "src"),
        os.path.join(project_name, "src", "components")
    ]
    
    for d in dirs:
        os.makedirs(d, exist_ok=True)
        
    # App.pysx
    app_pysx = """
import Greet
import Counter
import MathBox
import Condition
import Card

def App():
    return <div class="app-container">
        <h1>Welcome to PYSX ✨</h1>
        <p>This is a modern full-stack web app written in Python syntax.</p>
        
        <div class="card-grid">
            <Card title="Props Demo">
                <Greet name="PYSX Developer" />
            </Card>

            <Card title="State & Events Demo">
                <Counter />
            </Card>

            <Card title="Math Expressions">
                <MathBox a={10} b={5} />
            </Card>

            <Card title="Conditional UI">
                <Condition flag={true} />
            </Card>
        </div>
    </div>
"""

    # Greet.pysx
    greet_pysx = """
def Greet(name):
    return <div>
        <h3>Hello {name}!</h3>
        <p>Props are working perfectly.</p>
    </div>
"""

    # Counter.pysx
    counter_pysx = """
def inc():
    alert("Clicked!")

def Counter():
    return <div>
        <p>Variables and events supported in PYSX v2!</p>
        <button onClick={inc}>Click Me</button>
    </div>
"""

    # MathBox.pysx
    mathbox_pysx = """
def MathBox(a, b):
    return <div>
        <p>a = {a}</p>
        <p>b = {b}</p>
        <strong>Result of a * b = {a * b}</strong>
    </div>
"""

    # Condition.pysx
    condition_pysx = """
def Condition(flag):
    return <div>
        <p>Current flag: {flag}</p>
        {flag && <div class="badge">Flag is TRUE ✅</div>}
    </div>
"""

    # Card.pysx
    card_pysx = """
def Card(title, children):
    return <div class="card">
        <h2 class="card-title">{title}</h2>
        <div class="card-body">
            {children}
        </div>
    </div>
"""

    # main.js
    main_js = """
// Auto-generated PYSX runtime
import App from "../dist/bundle.js";

const root = ReactDOM.createRoot(document.getElementById("root"));
root.render(React.createElement(App));
"""

    # index.html
    index_html = """
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>PYSX App</title>
    <script src="https://unpkg.com/react@18/umd/react.development.js"></script>
    <script src="https://unpkg.com/react-dom@18/umd/react-dom.development.js"></script>
    <link rel="stylesheet" href="./style.css">
</head>
<body>
    <div id="root"></div>
    <script src="../dist/bundle.js"></script>
    <script>
        const root = ReactDOM.createRoot(document.getElementById("root"));
        root.render(React.createElement(App));
    </script>
</body>
</html>
"""

    # style.css
    style_css = """
body {
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
    margin: 0;
    padding: 20px;
    background-color: #f0f2f5;
    color: #333;
}
.app-container {
    max-width: 800px;
    margin: 0 auto;
}
.card-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    gap: 20px;
    margin-top: 20px;
}
.card {
    background: white;
    border-radius: 8px;
    padding: 20px;
    box-shadow: 0 2px 4px rgba(0,0,0,0.1);
}
.card-title {
    margin-top: 0;
    font-size: 1.25rem;
    color: #1a1a1a;
    border-bottom: 2px solid #f0f2f5;
    padding-bottom: 10px;
}
button {
    background: #0066cc;
    color: white;
    border: none;
    padding: 8px 16px;
    border-radius: 4px;
    cursor: pointer;
}
button:hover {
    background: #0052a3;
}
"""

    # build.sh
    build_sh = """#!/bin/bash
GREEN='\\033[0;32m'
BLUE='\\033[0;34m'
RED='\\033[0;31m'
NC='\\033[0m'

echo -e "${BLUE}🚀 Building PYSX App...${NC}"

mkdir -p dist
compiled_files=()

COMPILER="pysx"

if ! command -v "$COMPILER" >/dev/null 2>&1; then
    echo -e "${RED}❌ Error: pysx compiler not found. Please install the pysx pip package globally.${NC}"
    exit 1
fi

while IFS= read -r file; do
    FILENAME=$(basename -- "$file")
    BASENAME="${FILENAME%.*}"
    OUTPUT_PATH="dist/${BASENAME}.js"
    
    echo "⚡ Compiling: $file -> $OUTPUT_PATH"
    "$COMPILER" "$file" -o "$OUTPUT_PATH"
    
    compiled_files+=("$OUTPUT_PATH")
done < <(find src -name "*.pysx" -type f)

if [ ${#compiled_files[@]} -gt 0 ]; then
    echo -e "\\n🔗 Linking into dist/bundle.js..."
    "$COMPILER" --bundle "${compiled_files[@]}" dist/bundle.js
    echo -e "\\n${GREEN}✅ Success! Build complete.${NC}"
else
    echo -e "${RED}❌ Error: No .pysx files found in src directory.${NC}"
fi
"""

    create_file(os.path.join(project_name, "src", "App.pysx"), app_pysx)
    create_file(os.path.join(project_name, "src", "components", "Greet.pysx"), greet_pysx)
    create_file(os.path.join(project_name, "src", "components", "Counter.pysx"), counter_pysx)
    create_file(os.path.join(project_name, "src", "components", "MathBox.pysx"), mathbox_pysx)
    create_file(os.path.join(project_name, "src", "components", "Condition.pysx"), condition_pysx)
    create_file(os.path.join(project_name, "src", "components", "Card.pysx"), card_pysx)
    
    create_file(os.path.join(project_name, "src", "main.js"), main_js)
    create_file(os.path.join(project_name, "public", "index.html"), index_html)
    create_file(os.path.join(project_name, "public", "style.css"), style_css)
    
    build_script = os.path.join(project_name, "build.sh")
    create_file(build_script, build_sh)
    os.chmod(build_script, 0o755) # Make executable
    
    print(f"\\n✅ Project '{project_name}' created successfully!")
    print("\\nNext steps:")
    print(f"  cd {project_name}")
    print("  ./build.sh")
    print("  open public/index.html")

if __name__ == "__main__":
    main()
