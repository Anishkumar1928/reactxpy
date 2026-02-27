function List(props){
  
var items = [ "a" , "b" , "c" ] return React.createElement("ul", null,
  items.map(( x) => React.createElement("li", null, "{x}"))) 
}

export default List;
