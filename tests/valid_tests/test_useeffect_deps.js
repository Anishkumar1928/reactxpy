function App(props){
  
const [count, setCount] = ReactXPy.useState( 0 ); ReactXPy.useEffect( ( ) => console.log ( count ) , [ count ] ); return React.createElement("div", null,
  count) 
}

export default App;
