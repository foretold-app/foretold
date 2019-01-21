open Utils;
open Rationale;
open Queries;
open HandsOnTable;
open MeasurableColumns;
open Table;
open MeasurableIndexTable__Styles;

let component = ReasonReact.statelessComponent("MeasurableIndexTable");

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
  if (!measurable.isLocked && pastExpectedResolutionDate) {
    PENDING_REVIEW;
  } else if (measurable.isLocked) {
    CLOSED;
  } else {
    OPEN;
  };
};

let compareSimilarMeasurables =
    (measurableA: Queries.measurable, measurableB: Queries.measurable) =>
  switch (
    measurableA.expectedResolutionDate,
    measurableB.expectedResolutionDate,
  ) {
  | (Some(a), Some(b)) => MomentRe.Moment.isAfter(a, b) ? 1 : (-1)
  | _ => 1
  };

let compareMeasurables =
    (measurableA: Queries.measurable, measurableB: Queries.measurable) =>
  switch (status(measurableA), status(measurableB)) {
  | (a, b) when a == b => compareSimilarMeasurables(measurableA, measurableB)
  | (a, b) => statusInt(a) > statusInt(b) ? (-1) : 1
  };

let dateFinder = (~measurable: Queries.measurable, head, p, dateFn) => {
  let date =
    measurable
    |> Option.Infix.(
         e =>
           dateFn(e) <$> MomentRe.Moment.format("L") |> Option.default("")
       );
  <div className=Styles.statusRow>
    <h3> {head |> ste} </h3>
    <p> {p ++ date |> ste} </p>
  </div>;
};

let dateStatus = (~measurable: Queries.measurable) => {
  let m = measurable;
  switch (status(m)) {
  | OPEN =>
    dateFinder(~measurable=m, "Open", "Closes ~", e =>
      e.expectedResolutionDate
    )
  | PENDING_REVIEW =>
    dateFinder(~measurable=m, "Judgement Pending", "Pending since ", e =>
      e.expectedResolutionDate
    )
  | CLOSED =>
    dateFinder(~measurable=m, "Closed", "Closed on ", e => e.lockedAt)
  };
};

let fileExcel = Antd_IconName.fileExcel;
let make = (~measurables: array(Queries.measurable), _children) => {
  ...component,
  render: _self => {
    let _measurables =
      Belt.SortArray.stableSortBy(measurables, compareMeasurables);
    <UseRouterForLinks>
      <div className=Styles.group>
        {
          _measurables
          |> Array.map(m =>
               <div className=Styles.row>
                 <div className=Styles.mainColumn>
                   <div className=Styles.mainColumnTop>
                     <a href={"/measurables/" ++ m.id}> {m.name |> ste} </a>
                   </div>
                   <div className=Styles.mainColumnBottom>
                     <div className=Styles.item>
                       Option.Infix.(
                         m.creator
                         <$> (
                           c =>
                             <a href={"/agents/" ++ c.id}>
                               {c.name |> Option.default("") |> ste}
                             </a>
                         )
                         |> Option.default("" |> ste)
                       )
                     </div>
                     <div className=Styles.item>
                       <span>
                         {
                           m.measurementCount
                           |> Option.default(0)
                           |> string_of_int
                           |> ste
                         }
                       </span>
                       <span> {"/" |> ste} </span>
                       <span>
                         {
                           m.measurerCount
                           |> Option.default(0)
                           |> string_of_int
                           |> ste
                         }
                       </span>
                     </div>
                   </div>
                 </div>
                 <div className=Styles.rightColumn>
                   {dateStatus(~measurable=m)}
                 </div>
               </div>
             )
          |> ReasonReact.array
        }
      </div>
    </UseRouterForLinks>;
  },
};