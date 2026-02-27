function ActiveUsers(props){
  
var users = [ { active : true , name : "John" } ] return React.createElement("ul", null,
  users.filter(( u) => u.active).map(( u) => React.createElement("li", null, "{u.name}"))) 
}

export default ActiveUsers;
