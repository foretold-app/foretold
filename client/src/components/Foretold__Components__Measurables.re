open Utils;
open Foretold__GraphQL;

module BasicTable = {
  let component = ReasonReact.statelessComponent("MeasurableIndexTable");

  let make =
      (
        ~measurables: array(DataModel.Measurable.t),
        ~showExtraData: bool,
        ~loggedInUser: Queries.User.t,
        ~onSelect=(m: DataModel.Measurable.t) =>
                    DataModel.Url.push(
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
          |> E.A.fmap((m: DataModel.Measurable.t) => {
               let userAgentId =
                 loggedInUser
                 |> E.O.bind(_, (r: Queries.User.user) => r.agent)
                 |> E.O.fmap((r: Queries.User.agent) => r.id);
               let measurableAgentId =
                 m.creator |> E.O.fmap((r: DataModel.Agent.t) => r.id);
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
                     {
                       [|
                         E.React.showIf(
                           showExtraData,
                           MeasurableTableStyles.series(~m),
                         ),
                         E.React.showIf(
                           showExtraData,
                           MeasurableTableStyles.creatorLink(~m),
                         ),
                         MeasurableTableStyles.measurements(~m),
                         MeasurableTableStyles.measurers(~m),
                         E.React.showIf(
                           isSame,
                           MeasurableTableStyles.editLink(~m),
                         ),
                         E.React.showIf(
                           isSame,
                           MeasurableTableStyles.archiveOption(~m),
                         ),
                       |]
                       |> ReasonReact.array
                     }
                   </div>
                 </div>
                 <div className=PrimaryTableStyles.rightColumn>
                   <Foretold__Components__Measurable.StatusDisplay
                     measurable=m
                     dateDisplay=WHOLE
                   />
                 </div>
               </div>;
             })
          |> ReasonReact.array
        }
      </div>,
  };
};