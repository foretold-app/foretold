// @todo: To make a component.
let make = (~notebookRedux: NotebookRedux.t) => {
  switch (notebookRedux.state.selectedMeasurableId) {
  | Some(measurableId) =>
    Some(
      MeasurableGet.component(~id=measurableId)
      |> E.F.apply((measurable: Types.measurable) => {
           let defaultValueText =
             measurable.recentMeasurement
             |> E.O.bind(_, (r: Types.measurement) => r.valueText)
             |> E.O.default("");

           <div>
             <SLayout container=`fluidLeft>
               <ForetoldComponents.PageCard.Body>
                 <MeasurementForm
                   measurable
                   defaultValueText
                   key={measurable.id}
                 />
               </ForetoldComponents.PageCard.Body>
             </SLayout>
             <SLayout.Container container=`fluidLeft>
               <MeasurableBottomSection.Inner
                 measurable
                 key={measurable.id}
                 block=`inside
                 leaderboardColumns=LeaderboardTable.Columns.notebooks
               />
             </SLayout.Container>
           </div>;
         }),
    )
  | None => None
  };
};