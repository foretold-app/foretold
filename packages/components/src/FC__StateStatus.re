module State = {
  type t =
    | OPEN
    | PENDING
    | RESOLVED;

  let toColor = (t: t) =>
    switch (t) {
    | OPEN => FC__Colors.Statuses.green
    | PENDING => FC__Colors.Statuses.yellow
    | RESOLVED => FC__Colors.Statuses.red
    };

  let toText = (t: t) =>
    switch (t) {
    | OPEN => "Open"
    | PENDING => "Pending"
    | RESOLVED => "Resolved"
    };
};

module Style = {
  let token = (state, fontSize') =>
    Css.(
      style([
        FC__Colors.FontWeights.heavy,
        color(State.toColor(state)),
        marginRight(`em(1.0)),
        fontSize(fontSize'),
      ])
    );

  let text = fontSize' =>
    Css.(style([color(FC__Colors.accentBlue), fontSize(fontSize')]));
};

let make = (~state: State.t, ~fontSize=`em(0.9), ()) =>
  <div>
    <span className={Style.token(state, fontSize)}>
      {State.toText(state) |> ReasonReact.string}
    </span>
    <span className={Style.text(fontSize)}>
      {"Closes in 8 days" |> ReasonReact.string}
    </span>
  </div>;