function Input(props){
  
return React.createElement("input", { onChange: ( e) => console.log(e.target.value) }) 
}

export default Input;
