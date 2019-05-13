open Utils;

type dateDisplay =
  | TOP
  | BOTTOM
  | WHOLE;

type status =
  | OPEN
  | PENDING_REVIEW
  | JUDGED;

let statusColor = (~measurable: Context.Primary.Measurable.t) => {
  open Css;
  let main = [padding2(~v=`px(1), ~h=`px(8)), borderRadius(`px(4))];
  let statusSpecific =
    switch (Context.Primary.Measurable.toStatus(measurable)) {
    | `OPEN => [background(`hex("bff5bd"))]
    | `JUDGEMENT_PENDING => [background(`hex("fff8da"))]
    | `JUDGED => [background(`hex("ead7f3"))]
    };
  style([main, statusSpecific] |> E.L.concat);
};

let statusRow =
  Css.(
    style([
      flex(1),
      selector(
        " h3",
        [
          marginTop(px(2)),
          marginBottom(px(-1)),
          fontSize(px(16)),
          color(`hex("666")),
        ],
      ),
      selector(
        " p",
        [marginBottom(px(2)), fontSize(`px(12)), color(`hex("aaa"))],
      ),
    ])
  );

let dateFinder = (head, p, date, dateDisplay) => {
  let date = date |> E.O.fmap(E.M.goFormat_simple) |> E.O.default("");
  switch (dateDisplay) {
  | TOP => head |> ste
  | BOTTOM => p ++ date |> ste
  | WHOLE =>
    <div className=statusRow>
      {head |> ste |> E.React.inH3}
      {p ++ date |> ste |> E.React.inP}
    </div>
  };
};

let statusShow = (~measurable: Context.Primary.Measurable.t, ~dateDisplay) =>
  switch (Context.Primary.Measurable.toStatus(measurable)) {
  | `OPEN =>
    dateFinder(
      "Open",
      "Closes ~",
      measurable.expectedResolutionDate,
      dateDisplay,
    )
  | `JUDGEMENT_PENDING =>
    dateFinder(
      "Judgement Pending",
      "Pending since ",
      measurable.expectedResolutionDate,
      dateDisplay,
    )
  | `JUDGED =>
    dateFinder("Judged", "Judged on ", measurable.stateUpdatedAt, dateDisplay)
  };
let component = ReasonReact.statelessComponent("MeasurableShow");

let make =
    (
      ~measurable: Context.Primary.Measurable.t,
      ~dateDisplay=WHOLE,
      ~withStatusColorSurrounding=false,
      _children,
    ) => {
  ...component,
  render: _self => {
    let finder = statusShow(~measurable, ~dateDisplay);
    withStatusColorSurrounding ?
      <div className={statusColor(~measurable)}> finder </div> : finder;
  },
};