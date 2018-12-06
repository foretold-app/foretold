open Utils;
open Rationale;
open Queries;
open HandsOnTable;
open MeasurableColumns;
open Table;

let transformations = [
  link,
  nameAsText,
  measurementCount,
  measurerCount,
  expectedResolutionDate,
  creator,
  isLocked,
];

let component = ReasonReact.statelessComponent("MeasurableIndexTable");

module Styles = {
  open Css;
  let row =
    style([
      width(`percent(100.0)),
      backgroundColor(hex("f6f6f6")),
      marginBottom(px(2)),
      selector(" h2", [marginTop(px(2))]),
      display(`flex),
      flexDirection(`row),
      paddingLeft(px(5)),
      paddingRight(px(5)),
    ]);

  let nameRow =
    style([
      flex(4),
      padding(px(2)),
      selector(
        " a",
        [
          color(`hex("666")),
          fontSize(px(14)),
          selector(":hover", [backgroundColor(hex("ddd"))]),
        ],
      ),
    ]);
  let ownerRow = style([flex(1), padding(px(2))]);
  let statsRow = style([flex(1), padding(px(2))]);
  let columnHeader = style([color(hex("222"))]);
  let statusRow =
    style([
      flex(1),
      padding(px(2)),
      selector(
        " h3",
        [marginTop(px(2)), marginBottom(px(0)), fontSize(px(14))],
      ),
      selector(" p", [marginBottom(px(2)), fontSize(px(12))]),
    ]);
  let menu = style([lineHeight(px(64))]);
};

type status =
  | OPEN
  | PENDING_REVIEW
  | CLOSED;

let statusInt = (status: status) =>
  switch (status) {
  | OPEN => 2
  | PENDING_REVIEW => 1
  | CLOSED => 0
  };

let status = (measurable: Queries.measurable) => {
  let pastExpectedResolutionDate =
    switch (measurable.expectedResolutionDate) {
    | None => false
    | Some(e) => MomentRe.Moment.isAfter(MomentRe.momentNow(), e)
    };
  if (! measurable.isLocked && pastExpectedResolutionDate) {
    PENDING_REVIEW;
  } else if (measurable.isLocked) {
    CLOSED;
  } else {
    OPEN;
  };
};

let compareMeasurables =
    (measurableA: Queries.measurable, measurableB: Queries.measurable) =>
  switch (status(measurableA), status(measurableB)) {
  | (OPEN, OPEN) =>
    switch (
      measurableA.expectedResolutionDate,
      measurableB.expectedResolutionDate,
    ) {
    | (Some(a), Some(b)) => MomentRe.Moment.isAfter(a, b) ? 1 : (-1)
    | _ => 1
    }
  | (PENDING_REVIEW, PENDING_REVIEW) =>
    switch (
      measurableA.expectedResolutionDate,
      measurableB.expectedResolutionDate,
    ) {
    | (Some(a), Some(b)) => MomentRe.Moment.isAfter(a, b) ? 1 : (-1)
    | _ => 1
    }
  | (CLOSED, CLOSED) =>
    switch (measurableA.lockedAt, measurableB.lockedAt) {
    | (Some(a), Some(b)) => MomentRe.Moment.isAfter(a, b) ? 1 : (-1)
    | _ => 1
    }
  | _ =>
    statusInt(status(measurableA)) > statusInt(status(measurableB)) ?
      (-1) : 1
  };

let fileExcel = Antd_IconName.fileExcel;
let make = (~measurables: array(Queries.measurable), _children) => {
  ...component,
  render: _self => {
    let bar = Belt.SortArray.stableSortBy(measurables, compareMeasurables);
    <UseRouterForLinks>
      <div>
        <div className=(Styles.row ++ " " ++ Styles.columnHeader)>
          <div className=Styles.nameRow> ("Name" |> ste) </div>
          <div className=Styles.ownerRow> ("Owner" |> ste) </div>
          <div className=Styles.statsRow>
            ("Measurements/Measurers" |> ste)
          </div>
          <div className=Styles.statusRow> ("Status" |> ste) </div>
        </div>
        (
          bar
          |> Array.map(m =>
               <div className=Styles.row>
                 <div className=Styles.nameRow>
                   <a href=("/measurables/" ++ m.id)> (m.name |> ste) </a>
                 </div>
                 <div className=Styles.ownerRow>
                   Option.Infix.(
                     m.creator
                     <$> (
                       c =>
                         <a href=("/agents/" ++ c.id)>
                           (c.name |> Option.default("") |> ste)
                         </a>
                     )
                     |> Option.default("" |> ste)
                   )
                 </div>
                 <div className=Styles.statsRow>
                   <div>
                     <span>
                       (
                         m.measurementCount
                         |> Option.default(0)
                         |> string_of_int
                         |> ste
                       )
                     </span>
                     <span> ("/" |> ste) </span>
                     <span>
                       (
                         m.measurerCount
                         |> Option.default(0)
                         |> string_of_int
                         |> ste
                       )
                     </span>
                   </div>
                 </div>
                 (
                   switch (status(m)) {
                   | OPEN =>
                     let date =
                       m
                       |> Option.Infix.(
                            (
                              e =>
                                e.expectedResolutionDate
                                <$> MomentRe.Moment.format("L")
                                |> Option.default("")
                            )
                          );
                     <div className=Styles.statusRow>
                       <h3> ("Open" |> ste) </h3>
                       <p> ("Closes ~" ++ date |> ste) </p>
                     </div>;
                   | PENDING_REVIEW =>
                     let date =
                       m
                       |> Option.Infix.(
                            (
                              e =>
                                e.expectedResolutionDate
                                <$> MomentRe.Moment.format("L")
                                |> Option.default("")
                            )
                          );
                     <div className=Styles.statusRow>
                       <h3> ("Judgement Pending" |> ste) </h3>
                       <p> ("Pending since " ++ date |> ste) </p>
                     </div>;
                   | CLOSED =>
                     let date =
                       m
                       |> Option.Infix.(
                            (
                              e =>
                                e.lockedAt
                                <$> MomentRe.Moment.format("L")
                                |> Option.default("")
                            )
                          );
                     <div className=Styles.statusRow>
                       <h3> ("Closed" |> ste) </h3>
                       <p> ("Closed on " ++ date |> ste) </p>
                     </div>;
                   }
                 )
               </div>
             )
          |> ReasonReact.array
        )
      </div>
    </UseRouterForLinks>;
  },
  /* |> Array.sort((a, b) => 1) */
  /* let foo =  */
};