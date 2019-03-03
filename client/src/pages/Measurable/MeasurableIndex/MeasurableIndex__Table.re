open Utils;
open Rationale;
open Queries;
open HandsOnTable;
open MeasurableColumns;
open Table;

let component = ReasonReact.statelessComponent("MeasurableIndexTable");

let make =
    (
      ~measurables: array(DataModel.measurable),
      ~showExtraData: bool,
      _children,
    ) => {
  ...component,
  render: _self => {
    let _measurables =
      MeasurableTableStyles.sortMeasurables(measurables)
      |> Js.Array.filter((e: DataModel.measurable) =>
           PrimaryTableBase.status(e) != ARCHIVED
         );
    <div className=PrimaryTableStyles.group>
      {
        _measurables
        |> Array.map(m =>
             <div
               className={PrimaryTableStyles.row(m)}
               onClick={
                 _e =>
                   ReasonReact.Router.push(
                     "/c/"
                     ++ (m.channel |> Option.default("general"))
                     ++ "/m/"
                     ++ m.id,
                   )
               }>
               <div className=PrimaryTableStyles.mainColumn>
                 <div className=PrimaryTableStyles.mainColumnTop>
                   {MeasurableTableStyles.link(~m)}
                 </div>
                 <div className=PrimaryTableStyles.mainColumnBottom>
                   {
                     showExtraData ?
                       MeasurableTableStyles.series(~m) : ReasonReact.null
                   }
                   {
                     showExtraData ?
                       MeasurableTableStyles.creatorLink(~m) :
                       ReasonReact.null
                   }
                   {MeasurableTableStyles.measurements(~m)}
                   {MeasurableTableStyles.measurers(~m)}
                 </div>
               </div>
               <div className=PrimaryTableStyles.rightColumn>
                 {MeasurableTableStyles.dateStatus(~measurable=m)}
               </div>
             </div>
           )
        |> ReasonReact.array
      }
    </div>;
  },
};