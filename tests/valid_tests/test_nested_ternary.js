function Grade(props){
  
return React.createElement("span", null,
  props.score > 90 ? "A" : props.score > 80 ? "B" : "C") 
}

export default Grade;
