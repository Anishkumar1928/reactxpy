function TodoItem(props){
  
return React.createElement("li", { className: props.completed ? "done" : "" },
  React.createElement("input", { checked: props.completed, type: "checkbox" }),
  React.createElement("span", null,
    props.text),
  React.createElement("button", { onClick: () => console.log("delete") },
    "Delete")) 
}

export default TodoItem;
