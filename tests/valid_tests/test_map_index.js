function Items(props){
  
var items = [ "a" , "b" ] return React.createElement("ul", null,
  items.map(( x, i) => React.createElement("li", {key: i}, "{x}"))) 
}

export default Items;
