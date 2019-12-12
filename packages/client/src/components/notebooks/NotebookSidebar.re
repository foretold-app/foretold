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
                   measurableId={measurable.id}
                   isCreator=false
                   defaultValueText
                   key={measurable.id}
                 />
               </FC.PageCard.Body>
             </SLayout>
             <MeasurableBottomSection
               measurableId={measurable.id}
               channelId=None
               key={measurable.id}
             />
           </div>;
         }),
    )
  | None => None
  };
};