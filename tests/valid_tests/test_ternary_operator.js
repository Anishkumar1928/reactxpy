function Status(props){
  
return React.createElement("span", null,
  props.active ? "Active" : "Inactive") 
}

export default Status;
