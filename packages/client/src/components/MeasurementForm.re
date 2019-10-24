[@bs.config {jsx: 3}];

let component = ReasonReact.statelessComponent("MeasurementForm");

let make =
    (
      ~measurable: Types.measurable,
      ~measurableId: string,
      ~isCreator: bool,
      _children,
    ) => {
  ...component,
  render: _ =>
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
        | _ => ReasonReact.null
        }
      }
    </Providers.AppContext.Consumer>,
};
