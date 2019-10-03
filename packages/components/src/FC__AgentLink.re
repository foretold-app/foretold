[@bs.config {jsx: 3}];

module Link = {
  let component = ReasonReact.statelessComponent(__MODULE__ ++ " link");
  let make =
    FC__Link.Jsx2.make(
      ~className=
        Css.(
          style([
            display(`inlineBlock),
            color(FC__Settings.textDark),
            hover([color(FC__Settings.darkLink)]),
          ])
        ),
      ~isDisabled=false,
    );
};

module Styles = {
  open Css;
  let small = style([fontSize(`em(0.8)), marginTop(`em(0.1))]);
  let by =
    style([
      marginRight(`em(0.4)),
      color(FC__Settings.accentBlue),
      fontSize(`em(0.8)),
    ]);
  let imageCropper =
    style([
      width(`em(1.)),
      height(`em(1.)),
      marginRight(`em(0.4)),
      marginTop(`em(0.1)),
      overflow(`hidden),
      position(`relative),
      borderRadius(`percent(20.)),
      display(`inlineBlock),
    ]);
  let image =
    style([
      display(`inline),
      margin2(~v=`zero, ~h=`auto),
      height(`auto),
      width(`percent(100.)),
    ]);
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
  let make = (~agent: Agent.t) =>
    <FC__Link onClick={Agent.onClick(agent)}>
      <div className=Styles.imageCropper>
        <img
          src={
            Agent.image(agent)
            |> Rationale.Option.default(BotDefaultImage.botDefault)
          }
          className=Styles.image
        />
      </div>
        {Agent.name(agent) |> ReasonReact.string}
    </FC__Link>;
};

let component = ReasonReact.statelessComponent(__MODULE__);

[@react.component]
let make = (~agent: Agent.t) => {
  FC__Base.(
    switch (Agent.owner(agent)) {
    | Some(owner) =>
      <Div flexDirection=`column>
        <Div flex={`num(1.0)}> <SubItem agent /> </Div>
        <Div flex={`num(1.0)} className=Styles.small>
          <span className=Styles.by> {"by " |> ReasonReact.string} </span>
          <SubItem agent=owner />
        </Div>
      </Div>
    | None => <SubItem agent />
    }
  );
};

module Jsx2 = {
  let component = ReasonReact.statelessComponent("Link");

  let make = (~agent: Agent.t, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~agent, ()),
      children,
    );
};