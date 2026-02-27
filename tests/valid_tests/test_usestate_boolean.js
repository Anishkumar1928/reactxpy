function Toggle(props){
  
const [on, setOn] = ReactXPy.useState( false ); return React.createElement("p", null,
  on ? "Yes" : "No") 
}

export default Toggle;
