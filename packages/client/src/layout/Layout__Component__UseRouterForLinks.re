open ReactEvent;

let component = ReasonReact.statelessComponent("UseRouterForLinks");

module Styles = {
  open Css;
  let outer = style([height(`percent(100.0))]);
};

let make = (~onClickData=e => (), children) => {
  let click = event =>
    if (Synthetic.target(event)##localName == "a") {
      let data: Js.nullable(string) =
        Synthetic.target(event)##getAttribute("data");
      switch (Js.Nullable.toOption(data)) {
      | Some(r) =>
        Synthetic.preventDefault(event);
        onClickData(r);
      | None =>
        let url = Synthetic.target(event)##getAttribute("href");
        Synthetic.preventDefault(event);
        ReasonReact.Router.push(url);
      };
    };
  {
    ...component,
    render: _ => <div onClick=click className=Styles.outer> ...children </div>,
  };
};