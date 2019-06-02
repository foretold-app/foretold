open FC;
open FC.Base;

let numbers = [
  0.00000000001,
  0.00000001,
  0.00001,
  0.01,
  0.0,
  1.0,
  100.0,
  100.5,
  1000000.0,
  100000000000.0,
  100000000000000000.0,
  10000000000000000000000.0,
];

let colorBoxStyle =
  Css.(
    style([
      flexBasis(`px(220)),
      flexGrow(1.),
      height(`px(90)),
      marginRight(`px(35)),
      marginBottom(`px(35)),
      textAlign(`center),
    ])
  );

let colorContainer = bgColor =>
  Css.(
    style([
      display(`flex),
      flexWrap(`wrap),
      padding(`em(1.5)),
      backgroundColor(bgColor),
    ])
  );

module NumbersDisplay = {
  let component = ReasonReact.statelessComponent(__MODULE__);
  let make = _children => {
    ...component,
    render: self =>
      <PageCard>
        <PageCard.HeaderRow>
          <PageCard.HeaderRow.Title>
            "Numbers"->React.string
          </PageCard.HeaderRow.Title>
        </PageCard.HeaderRow>
        <div>
          (
            numbers
            |> E.L.fmap(n =>
                 <div key={n |> Js.Float.toString}>
                   n->Js.Float.toString->React.string
                 </div>
               )
            |> E.L.toArray
          )
          ->React.array
        </div>
      </PageCard>,
  };
};

let entry =
  EntryTypes.(entry(~title="Numbers", ~render=() => <NumbersDisplay />));