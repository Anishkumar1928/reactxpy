function Form(props){
  
const [email, setEmail] = ReactXPy.useState( "" );const [password, setPassword] = ReactXPy.useState( "" ); return React.createElement("form", { onSubmit: ( e) => e.preventDefault() },
  React.createElement("input", { onChange: ( e) => setEmail(e.target.value), type: "email", value: email }),
  React.createElement("input", { onChange: ( e) => setPassword(e.target.value), type: "password", value: password }),
  React.createElement("button", { type: "submit" },
    "Login")) 
}

export default Form;
