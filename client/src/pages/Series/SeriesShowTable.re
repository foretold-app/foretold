open Utils;
open Rationale;

let component = ReasonReact.statelessComponent("MeasurableIndexTable");

module TableS = MeasurableTableStyles;

let make =
    (
      ~measurables: array(DataModel.Measurable.t),
      ~selected: option(string),
      ~onClick,
      _children,
    ) => {
  ...component,
  render: _self => {
    let _measurables =
      TableS.sortMeasurables(measurables)
      |> E.JsArray.filter((e: DataModel.Measurable.t) =>
           PrimaryTableBase.status(e) != ARCHIVED
         );
    <div className=PrimaryTableStyles.group>
      {
        _measurables
        |> Js_array.sortInPlaceWith(
             (a: DataModel.Measurable.t, b: DataModel.Measurable.t) =>
             switch (a.descriptionEntity, b.descriptionEntity) {
             | (Some(aa), Some(bb)) => bb > aa ? (-1) : 1
             | _ => 1
             }
           )
        |> Array.map((m: DataModel.Measurable.t) =>
             <div
               className={SeriesShowTableStyles.row(Some(m.id) == selected)}
               onClick={_e => onClick(m.id)}>
               <div className=SeriesShowTableStyles.column>
                 {
                   TableS.nameEntityLink(
                     ~m,
                     ~className=PrimaryTableStyles.itemLink,
                   )
                   |> E.O.React.defaultNull
                 }
               </div>
               <div className=SeriesShowTableStyles.column>
                 {
                   TableS.propertyEntityLink(
                     ~m,
                     ~className=PrimaryTableStyles.propertyLink,
                   )
                   |> E.O.React.defaultNull
                 }
               </div>
               <div className=SeriesShowTableStyles.column>
                 {
                   switch (TableS.formatDate(m.descriptionDate)) {
                   | "" => ReasonReact.null
                   | e =>
                     <span className=PrimaryTableStyles.calDateO>
                       {e |> ste}
                     </span>
                   }
                 }
               </div>
               <div className=SeriesShowTableStyles.column>
                 {TableS.measurements(~m)}
                 {TableS.measurers(~m)}
               </div>
               <div className=SeriesShowTableStyles.column>
                 TableS.(TableS.dateStatusI(~measurable=m, ~dateDisplay=TOP))
               </div>
               <div className=SeriesShowTableStyles.column>
                 TableS.(
                   TableS.dateStatusI(~measurable=m, ~dateDisplay=BOTTOM)
                 )
               </div>
             </div>
           )
        |> ReasonReact.array
      }
    </div>;
  },
};