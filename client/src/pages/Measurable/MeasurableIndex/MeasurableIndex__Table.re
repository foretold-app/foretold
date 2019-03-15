open Utils;
open Rationale;

let component = ReasonReact.statelessComponent("MeasurableIndexTable");

let make =
    (
      ~measurables: array(DataModel.measurable),
      ~showExtraData: bool,
      ~loggedInUser: GetUser.t,
      ~onSelect=(m: DataModel.measurable) =>
                  Urls.pushToLink(
                    MeasurableShow(
                      m.channel |> E.O.default("general"),
                      m.id,
                    ),
                  ),
      _children,
    ) => {
  ...component,
  render: _self =>
    <div className=PrimaryTableStyles.group>
      {
        measurables
        |> E.A.fmap((m: DataModel.measurable) => {
             let userAgentId =
               loggedInUser
               |> E.O.bind(_, (r: GetUser.user) => r.agent)
               |> E.O.fmap((r: GetUser.agent) => r.id);
             let measurableAgentId =
               m.creator |> E.O.fmap((r: DataModel.agent) => r.id);
             let isSame =
               userAgentId == measurableAgentId && E.O.isSome(userAgentId);
             <div
               className={PrimaryTableStyles.row(m)}
               onClick={_e => onSelect(m)}
               key={m.id}>
               <div className=PrimaryTableStyles.mainColumn>
                 <div className=PrimaryTableStyles.mainColumnTop>
                   {MeasurableTableStyles.link(~m)}
                 </div>
                 <div className=PrimaryTableStyles.mainColumnBottom>
                   {showIf(showExtraData, MeasurableTableStyles.series(~m))}
                   {
                     showIf(
                       showExtraData,
                       MeasurableTableStyles.creatorLink(~m),
                     )
                   }
                   {MeasurableTableStyles.measurements(~m)}
                   {MeasurableTableStyles.measurers(~m)}
                   {showIf(isSame, MeasurableTableStyles.editLink(~m))}
                   {showIf(isSame, MeasurableTableStyles.archiveOption(~m))}
                 </div>
               </div>
               <div className=PrimaryTableStyles.rightColumn>
                 {MeasurableTableStyles.dateStatus(~measurable=m)}
               </div>
             </div>;
           })
        |> ReasonReact.array
      }
    </div>,
};