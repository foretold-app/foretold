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
             <SLayout container=`none>
               <FC.PageCard.Body>
                 <MeasurementForm
                   measurable
                   defaultValueText
                   key={measurable.id}
                 />
               </FC.PageCard.Body>
             </SLayout>
             <MeasurableBottomSection measurable key={measurable.id} />
           </div>;
         }),
    )
  | None => None
  };
};