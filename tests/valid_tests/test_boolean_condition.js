function Toggle(props){
  
return React.createElement("div", null,
  props.isOn && React.createElement("p", null, "Power is on")) 
}

export default Toggle;
