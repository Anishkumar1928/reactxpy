function Alert(props){
  
return React.createElement("div", null,
  props.show && React.createElement("span", null, "Warning")) 
}

export default Alert;
