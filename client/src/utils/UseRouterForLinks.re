let component = ReasonReact.statelessComponent("UseRouterForLinks");

let make = children => {
  let click = event =>
    if (ReactEvent.Synthetic.target(event)##localName == "a") {
      let url = ReactEvent.Synthetic.target(event)##getAttribute("href");
      ReactEvent.Synthetic.preventDefault(event);
      ReasonReact.Router.push(url);
    };
  {...component, render: _ => <div onClick=click> ...children </div>};
};