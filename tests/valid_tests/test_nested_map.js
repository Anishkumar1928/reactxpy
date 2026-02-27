function Grid(props){
  
var rows = [ [ 1 , 2 ] , [ 3 , 4 ] ] return React.createElement("div", null,
  rows.map(( row) => React.createElement("div", null, "{row.map(( cell) => React.createElement("span", null, "{cell}"))}"))) 
}

export default Grid;
