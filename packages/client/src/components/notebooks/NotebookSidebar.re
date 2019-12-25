// @todo: To make a component.
let make = (~notebookRedux: NotebookRedux.t) => {
  switch (notebookRedux.state.selectedMeasurableId) {
  | Some(measurableId) =>
    Some(
      MeasurableGet.component(~id=measurableId)
      |> E.F.apply((measurable: Types.measurable) => {
           let head = (~channelId: option(string), ~paginationPage, ()) => ReasonReact.null;

           let defaultValueText =
             measurable.recentMeasurement
             |> E.O.bind(_, (r: Types.measurement) => r.valueText)
             |> E.O.default("");

           <div>
             <SLayout
               head={head(~channelId=None, ~paginationPage=E.React2.null, ())}
               isFluid=true>
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