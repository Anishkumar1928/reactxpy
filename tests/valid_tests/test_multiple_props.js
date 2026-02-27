function Card(props){
  
return React.createElement("div", null,
  React.createElement("h2", null,
    props.title),
  React.createElement("p", null,
    props.content)) 
}

export default Card;
