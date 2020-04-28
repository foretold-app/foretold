[@react.component]
let make = (~measurable: Types.measurable, ~defaultValueText="") => {
  let context = React.useContext(Providers.app);
  switch (context.loggedUser) {
  | Some(loggedUser) =>
    <MeasurementCreate.Mutation>
      ...{(mutation, data) =>
        <MeasurementCdfInput
          measurable
          defaultValueText
          onSubmit={(
            (value, competitorType, description, valueText, asAgent),
          ) =>
            MeasurementCreate.mutate(
              mutation,
              measurable.id,
              value,
              competitorType,
              MeasurementCdfInput.MarkdownPreprocessing.addNewLines(description),
              valueText,
              asAgent,
            )
          }
          bots={loggedUser.bots}
          data
          loggedUser
        />
      }
    </MeasurementCreate.Mutation>
  | _ => <Null />
  };
};