open Utils;
open Rationale;

let component = ReasonReact.statelessComponent("MeasurablesSeriesTable");

module TableS = Foretold__Components__Measurable__Items;

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
           DataModel.Measurable.toStatus(e) != ARCHIVED
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
                   TableS.Entities.nameEntityLink(
                     ~m,
                     ~className=PrimaryTableStyles.itemLink,
                   )
                   |> E.O.React.defaultNull
                 }
               </div>
               <div className=SeriesShowTableStyles.column>
                 {
                   TableS.Entities.propertyEntityLink(
                     ~m,
                     ~className=PrimaryTableStyles.propertyLink,
                   )
                   |> E.O.React.defaultNull
                 }
               </div>
               <div className=SeriesShowTableStyles.column>
                 {
                   switch (TableS.formatDate(m.descriptionDate)) {
                   | "" => E.React.null
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
                 <Foretold__Components__Measurable.StatusDisplay
                   measurable=m
                   dateDisplay=TOP
                 />
               </div>
               <div className=SeriesShowTableStyles.column>
                 <Foretold__Components__Measurable.StatusDisplay
                   measurable=m
                   dateDisplay=BOTTOM
                 />
               </div>
             </div>
           )
        |> ReasonReact.array
      }
    </div>;
  },
};