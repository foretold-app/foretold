open Utils;

let formatDate = e =>
  e |> E.O.fmap(E.M.format(E.M.format_simple)) |> E.O.default("");

type dateDisplay =
  | TOP
  | BOTTOM
  | WHOLE;

type status =
  | OPEN
  | PENDING_REVIEW
  | ARCHIVED
  | JUDGED;

let statusColor = (~measurable: DataModel.Measurable.t) => {
  open Css;
  let main = [padding2(~v=`px(1), ~h=`px(8)), borderRadius(`px(4))];
  let statusSpecific =
    switch (DataModel.Measurable.toStatus(measurable)) {
    | OPEN => [background(`hex("bff5bd"))]
    | PENDING_REVIEW => [background(`hex("fff8da"))]
    | JUDGED => [background(`hex("ead7f3"))]
    | ARCHIVED => [background(`hex("cccccc"))]
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
  let date = formatDate(date);
  switch (dateDisplay) {
  | TOP => head |> ste
  | BOTTOM => p ++ date |> ste
  | WHOLE =>
    <div className=statusRow>
      <h3> {head |> ste} </h3>
      <p> {p ++ date |> ste} </p>
    </div>
  };
};

let statusShow = (~measurable: DataModel.Measurable.t, ~dateDisplay) =>
  switch (DataModel.Measurable.toStatus(measurable)) {
  | OPEN =>
    dateFinder(
      "Open",
      "Closes ~",
      measurable.expectedResolutionDate,
      dateDisplay,
    )
  | PENDING_REVIEW =>
    dateFinder(
      "Judgement Pending",
      "Pending since ",
      measurable.expectedResolutionDate,
      dateDisplay,
    )
  | ARCHIVED =>
    dateFinder(
      "Archived",
      "Archived on ",
      measurable.stateUpdatedAt,
      dateDisplay,
    )
  | JUDGED =>
    dateFinder("Judged", "Judged on ", measurable.stateUpdatedAt, dateDisplay)
  };
let component = ReasonReact.statelessComponent("MeasurableShow");

let make =
    (
      ~measurable: DataModel.Measurable.t,
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