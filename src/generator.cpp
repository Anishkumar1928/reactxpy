#include "generator.h"
#include "ast.h"
#include <cctype>

using namespace std;

// --------------------------------
// indentation helper
// --------------------------------
string indent(int level){
    return string(level*2,' ');
}

// --------------------------------
// escape JS string
// --------------------------------
string escapeJS(const string& input){
    string out;
    for(char c:input){
        if(c=='\\') out+="\\\\";
        else if(c=='"') out+="\\\"";
        else out+=c;
    }
    return out;
}

// --------------------------------
// prefix props variables
// value -> props.value
// --------------------------------
string prefixProps(string expr,
                   const vector<string>& params)
{
    for(const auto& p:params){

        size_t pos=0;

        while((pos=expr.find(p,pos))!=string::npos){

            bool left=(pos==0||!isalnum(expr[pos-1]));
            bool right=(pos+p.size()>=expr.size()
                        ||!isalnum(expr[pos+p.size()]));

            if(left&&right){
                expr.replace(pos,p.size(),"props."+p);
                pos+=6+p.size();
            }else pos++;
        }
    }
    return expr;
}

// --------------------------------
// INLINE JSX compiler
// <p>Hello</p>
// --------------------------------
string compileInlineJSX(const string& jsx){

    size_t ts=jsx.find('<')+1;
    size_t te=jsx.find('>');

    string tagStr = jsx.substr(ts, te - ts);
    string tag = tagStr;
    string props = "null";

    size_t spacePos = tagStr.find(' ');
    if (spacePos != string::npos) {
        tag = tagStr.substr(0, spacePos);
        string attrs = tagStr.substr(spacePos + 1);
        
        size_t classPos = attrs.find("class=");
        if (classPos != string::npos) {
            size_t q1 = attrs.find('"', classPos);
            size_t q2 = (q1 != string::npos) ? attrs.find('"', q1 + 1) : string::npos;
            if (q1 != string::npos && q2 != string::npos) {
                string className = attrs.substr(q1 + 1, q2 - q1 - 1);
                props = "{ className: \"" + className + "\" }";
            }
        }
    }

    size_t cs=te+1;
    size_t ce=jsx.find("</");

    string text=jsx.substr(cs,ce-cs);

    return "React.createElement(\""+tag+
           "\", " + props + ", \""+escapeJS(text)+"\")";
}

bool looksLikeJSX(const string& e){
    return e.find('<')!=string::npos;
}

// --------------------------------
// props → JS object
// --------------------------------
string propsToJS(shared_ptr<JSXNode> node,
                 const vector<string>& params)
{
    if(node->props.empty())
        return "null";

    string r="{ ";
    bool first=true;

    for(auto &p:node->props){

        if(!first) r+=", ";

        string key =
            (p.first=="class")
            ? "className"
            : p.first;

        r+=key+": ";

        // ⭐ EVENT HANDLER
        if(key.rfind("on",0)==0 && p.second.isExpression){
            r+=p.second.value;
        }
        else if(p.second.isExpression){
            r+=prefixProps(p.second.value,params);
        }
        else{
            r+="\""+escapeJS(p.second.value)+"\"";
        }

        first=false;
    }

    r+=" }";
    return r;
}

// --------------------------------
// JSX GENERATOR (CORE)
// --------------------------------
string generateJSX(shared_ptr<JSXNode> node,
                   const vector<string>& params,
                   int level)
{
    bool isComponent =
        !node->tag.empty() && isupper(node->tag[0]);

    string out;

    if(isComponent)
        out+="React.createElement("+node->tag+", ";
    else
        out+="React.createElement(\""+node->tag+"\", ";

    out+=propsToJS(node,params);

    if(node->selfClosing){
        out+=")";
        return out;
    }

    // ---------- CHILDREN ----------
    for(auto &child:node->children){

        out+=",\n"+indent(level);

        // TEXT
        if(child.type==CHILD_TEXT){
            out+="\""+escapeJS(child.value)+"\"";
        }

        // EXPRESSION ⭐ FIXED
        else if(child.type==CHILD_EXPR){

            string expr = prefixProps(child.value, params);

            // conditional JSX support
            if(looksLikeJSX(expr)){

                size_t jsxStart = expr.find('<');

                string before = expr.substr(0, jsxStart);
                string jsxPart = expr.substr(jsxStart);

                out += before +
                       compileInlineJSX(jsxPart);
            }
            else{
                out += expr;
            }
        }

        // CHILD ELEMENT
        else if(child.type==CHILD_ELEMENT){
            out+=generateJSX(child.element,
                             params,
                             level+1);
        }
    }

    out+=")";
    return out;
}

// wrapper
string generateJSX(shared_ptr<JSXNode> node){
    vector<string> empty;
    return generateJSX(node, empty, 1);
}

// --------------------------------
// FUNCTION GENERATOR
// --------------------------------
string generateFunction(const FunctionNode& fn){

    string out;

    // NORMAL FUNCTION (EVENT)
    if(fn.jsx == nullptr){

        out+="function "+fn.name+"(){\n";

        if(!fn.body.empty())
            out+="  "+fn.body+";\n";

        out+="}\n\n";
        return out;
    }

    // REACT COMPONENT
    out+="function "+fn.name+"(props){\n";
    out+="  return ";
    out+=generateJSX(fn.jsx, fn.params, 1);
    out+=";\n}\n\n";

    return out;
}

// --------------------------------
// IMPORT GENERATOR
// --------------------------------
string generateImport(const ImportNode& imp){
    return "import "+imp.module+
           " from \"./"+imp.module+".js\";\n";
}

// --------------------------------
// PROGRAM GENERATOR
// --------------------------------
string Generator::generate(const ProgramNode& program)
{
    string out;

    for(const auto& imp:program.imports)
        out+=generateImport(imp);

    if(!program.imports.empty())
        out+="\n";

    for(const auto& fn:program.functions)
        out+=generateFunction(fn);

    return out;
}