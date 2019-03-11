open Utils;
open Rationale;
open Rationale.Function.Infix;
open MomentRe;

let component = ReasonReact.statelessComponent("MeMeasurables");
let make = (~id: string, _children) => {
  ...component,
  render: _ =>
    (
      agent => {
        let measurables =
          agent##measurables
          |> E.A.Optional.concatSomes
          |> E.A.fmap(Queries.GetUserMeasurables.toMeasurable);
        <div className=PrimaryTableStyles.group>
          {
            measurables
            |> MeasurableTableStyles.sortMeasurables
            |> E.A.fmap((m: DataModel.measurable) =>
                 <div className={PrimaryTableStyles.row(m)} key={m.id}>
                   <div className=PrimaryTableStyles.mainColumn>
                     <div className=PrimaryTableStyles.mainColumnTop>
                       {MeasurableTableStyles.link(~m)}
                       <span
                         className=PrimaryTableStyles.creatorLinkLeftMargin
                       />
                       {MeasurableTableStyles.description(~m)}
                     </div>
                     <div className=PrimaryTableStyles.mainColumnBottom>
                       <span
                         className=PrimaryTableStyles.creatorLinkLeftMargin
                       />
                       {MeasurableTableStyles.measurements(~m)}
                       {MeasurableTableStyles.measurers(~m)}
                       {MeasurableTableStyles.expectedResolutionDate(~m)}
                       {MeasurableTableStyles.resolutionEndpoint(~m)}
                       {MeasurableTableStyles.editLink(~m)}
                       {MeasurableTableStyles.archiveOption(~m)}
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
      }
    )
    |> Queries.GetUserMeasurables.component(~id),
};