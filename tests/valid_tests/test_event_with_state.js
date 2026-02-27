function Counter(props){
  
const [count, setCount] = ReactXPy.useState( 0 ); return React.createElement("button", { onClick: () => setCount(count + 1) },
  count) 
}

export default Counter;
