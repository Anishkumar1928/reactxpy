function Counter(props){
  
const [count, setCount] = ReactXPy.useState( 0 ); ReactXPy.useEffect( ( ) => document.title = f "Count: {count}" , [ count ] ); return React.createElement("div", null,
  React.createElement("p", null,
    "Count: ",
    count),
  React.createElement("button", { onClick: () => setCount(count + 1) },
    "+"),
  React.createElement("button", { onClick: () => setCount(count - 1) },
    "-")) 
}

export default Counter;
