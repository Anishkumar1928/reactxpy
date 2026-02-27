function Modal(props){
  
return React.createElement("div", { className: props.isOpen ? "modal open" : "modal" },
  React.createElement("div", { className: "modal-content" },
    React.createElement("button", { onClick: props.onClose },
      "Close"))) 
}

export default Modal;
