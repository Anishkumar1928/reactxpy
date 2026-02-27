import Nav from "./Nav.js";
import Sidebar from "./Sidebar.js";

function App(props){
  
return React.createElement("div", null,
  React.createElement(Nav, null),
  React.createElement(Sidebar, null)) 
}

export default App;
