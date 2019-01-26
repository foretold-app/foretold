open Utils;

let component = ReasonReact.statelessComponent("EditMe");

let make = (~measurables: Js.Array.t(DataModel.measurable), _children) =>
  <UseRouterForLinks>
    <div className=PrimaryTableStyles.group>
      {
        measurables
        |> MeasurableTableStyles.sortMeasurables
        |> Array.map((m: DataModel.measurable) =>
             <div className=PrimaryTableStyles.row key={m.id}>
               <div className=PrimaryTableStyles.mainColumn>
                 <div className=PrimaryTableStyles.mainColumnTop>
                   {MeasurableTableStyles.link(~m)}
                   {MeasurableTableStyles.description(~m)}
                 </div>
                 <div className=PrimaryTableStyles.mainColumnBottom>
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
    </div>
  </UseRouterForLinks>;