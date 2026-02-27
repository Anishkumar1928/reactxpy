function Box(props){
  
return React.createElement("div", { onMouseOver: () => console.log("over") },
  "Hover") 
}

export default Box;
