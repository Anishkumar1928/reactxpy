function User(props){
  
const [user, setUser] = ReactXPy.useState( { name : "John" } ); return React.createElement("p", null,
  user.name) 
}

export default User;
