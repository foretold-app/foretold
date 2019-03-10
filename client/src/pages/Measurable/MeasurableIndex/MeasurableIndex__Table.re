open Utils;
open Rationale;

let component = ReasonReact.statelessComponent("MeasurableIndexTable");

let make =
    (
      ~measurables: array(DataModel.measurable),
      ~showExtraData: bool,
      ~loggedInUser: GetUser.t,
      _children,
    ) => {
  ...component,
  render: _self => {
    let _measurables =
      MeasurableTableStyles.sortMeasurables(measurables)
      |> Js.Array.filter((e: DataModel.measurable) =>
           PrimaryTableBase.status(e) != ARCHIVED
         );
    let isLoggedOn = loggedInUser |> E.O.isSome;
    <div className=PrimaryTableStyles.group>
      {
        _measurables
        |> E.A.fmap((m: DataModel.measurable) =>
             <div
               className={PrimaryTableStyles.row(m)}
               onClick={
                 _e =>
                   Urls.pushToLink(
                     MeasurableShow(
                       m.channel |> E.O.default("general"),
                       m.id,
                     ),
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