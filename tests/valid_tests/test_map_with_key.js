function Users(props){
  
var users = [ { id : 1 , name : "John" } ] return React.createElement("ul", null,
  users.map(( u) => React.createElement("li", {key: u.id}, "{u.name}"))) 
}

export default Users;
