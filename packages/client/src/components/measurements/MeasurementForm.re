[@react.component]
let make = (~measurable: Types.measurable, ~defaultValueText="") => {
  <Providers.AppContext.Consumer>
    ...{({loggedUser}) =>
      switch (loggedUser) {
      | Some(loggedUser) =>
        <MeasurementCreate.Mutation>
          ...{(mutation, data) =>
            <CdfInput
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
                  description,
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
      }
    }
  </Providers.AppContext.Consumer>;
};