[@bs.config {jsx: 3}];

[@react.component]
let make =
    (~measurable: Types.measurable, ~measurableId: string, ~isCreator: bool) => {
  <Providers.AppContext.Consumer>
    ...{({loggedUser}) =>
      switch (loggedUser) {
      | Some(loggedUser) =>
        <MeasurementCreate.Mutation>
          ...{(mutation, data) =>
            <CdfInput
              measurable
              onSubmit={(
                (value, competitorType, description, valueText, asAgent),
              ) =>
                MeasurementCreate.mutate(
                  mutation,
                  measurableId,
                  value,
                  competitorType,
                  description,
                  valueText,
                  asAgent,
                )
              }
              bots={loggedUser.bots}
              data
              isCreator
              loggedUser
            />
          }
        </MeasurementCreate.Mutation>
      | _ => <Null />
      }
    }
  </Providers.AppContext.Consumer>;
};