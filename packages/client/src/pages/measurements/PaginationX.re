open Style.Grid;

let component = ReasonReact.statelessComponent("PaginationX");
let make = _children => {
  ...component,
  render: _ =>
    <>
      <Div>
        <Div
          styles=[
            Css.style([
              FC.Base.BaseStyles.floatLeft,
              Css.paddingTop(`em(0.2)),
            ]),
          ]>
          <FC.Tab isActive=true>
            {"Predictions" |> ReasonReact.string}
          </FC.Tab>
          <FC.Tab isActive=false> {"Scores" |> ReasonReact.string} </FC.Tab>
        </Div>
      </Div>
    </>,
};