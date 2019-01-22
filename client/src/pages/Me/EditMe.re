open Utils;

let component = ReasonReact.statelessComponent("EditMe");

let make = (~measurables: Js.Array.t(Queries.measurable), _children) =>
  <div className=PrimaryTableStyles.group>
    {
      measurables
      |> MeasurableTableStyles.sortMeasurables
      |> Array.map((m: Queries.measurable) =>
           <div className=PrimaryTableStyles.row>
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
                 {MeasurableTableStyles.isLocked(~m)}
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