import NavLink from "./NavLink.js";

function Navbar(props){
  
return React.createElement("nav", null,
  React.createElement(NavLink, { href: "/" },
    "Home"),
  React.createElement(NavLink, { href: "/about" },
    "About")) 
}

export default Navbar;
