import Header from "./Header.js";
import Footer from "./Footer.js";

function App(props){
  
return React.createElement("div", null,
  React.createElement(Header, null),
  React.createElement(Footer, null)) 
}

export default App;
