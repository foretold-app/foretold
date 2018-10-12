open ReactEvent;

let component = ReasonReact.statelessComponent("UseRouterForLinks");

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
  {...component, render: _ => <div onClick=click> ...children </div>};
};