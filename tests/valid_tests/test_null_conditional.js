function Maybe(props){
  
return React.createElement("div", null,
  props.data ? props.data.name : "No props.data") 
}

export default Maybe;
