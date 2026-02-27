function Form(props){
  
return React.createElement("form", { onSubmit: ( e) => e.preventDefault() },
  "Submit") 
}

export default Form;
