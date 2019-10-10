[@bs.config {jsx: 3}];

module Styles = {
  open Css;
  let small = style([fontSize(`em(0.8)), marginTop(`em(0.1))]);
  let by =
    style([
      marginRight(`em(0.4)),
      color(FC__Settings.accentBlue),
      fontSize(`em(0.8)),
    ]);
  let avatar =
    style([marginTop(`em(0.3)), float(`left), marginRight(`em(0.4))]);
};

module Agent = {
  type user = {
    name: string,
    image: option(string),
    onClick: ReactEvent.Mouse.t => unit,
  }
  and bot = {
    name: string,
    image: option(string),
    onClick: ReactEvent.Mouse.t => unit,
    owner: option(t),
  }
  and t =
    | User(user)
    | Bot(bot);

  let onClick = agent =>
    switch (agent) {
    | User(u) => u.onClick
    | Bot(u) => u.onClick
    };

  let name = agent =>
    switch (agent) {
    | User(u) => u.name
    | Bot(u) => u.name
    };

  let image = agent =>
    switch (agent) {
    | User(u) => u.image
    | Bot(u) => u.image
    };

  let owner = agent =>
    switch (agent) {
    | User(_) => None
    | Bot(b) => b.owner
    };

  let makeUser = (~name: string, ~onClick=_ => (), ~image=?, ()): t =>
    User({name, image, onClick});

  let makeBot = (~name: string, ~onClick=_ => (), ~image=?, ~owner=?, ()): t =>
    Bot({name, image, onClick, owner});
};

module SubItem = {
  [@react.component]
  let make = (~agent: Agent.t, ~className) =>
    <FC__Link onClick={Agent.onClick(agent)} className>
      <div className=Styles.avatar>
        <FC__Avatar
          src={
            Agent.image(agent)
            |> Rationale.Option.default(BotDefaultImage.botDefault)
          }
        />
      </div>
      {Agent.name(agent) |> ReasonReact.string}
    </FC__Link>;
};

let component = ReasonReact.statelessComponent(__MODULE__);

[@react.component]
let make = (~agent: Agent.t, ~className="") => {
  FC__Base.(
    switch (Agent.owner(agent)) {
    | Some(owner) =>
      <Div flexDirection=`column>
        <Div flex={`num(1.0)}> <SubItem agent className /> </Div>
        <Div flex={`num(1.0)} className=Styles.small>
          <span className=Styles.by> {"by " |> ReasonReact.string} </span>
          <SubItem agent=owner className />
        </Div>
      </Div>
    | None => <SubItem agent className />
    }
  );
};

module Jsx2 = {
  let component = ReasonReact.statelessComponent("Link");

  let make = (~agent: Agent.t, ~className="", children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~agent, ~className, ()),
      children,
    );
};