let component = ReasonReact.statelessComponent("MeasurementForm");

let make =
    (
      ~loggedInUser: Types.user,
      ~measurable: Types.measurable,
      ~measurableId: string,
      ~isCreator: bool,
      _children,
    ) => {
  ...component,
  render: _ =>
    <MeasurementCreate.Mutation>
      ...{(mutation, data) =>
        <CdfInput
          measurable
          onSubmit={((value, competitorType, description, valueText)) =>
            MeasurementCreate.mutate(
              mutation,
              measurableId,
              value,
              competitorType,
              description,
              valueText,
            )
          }
          bots={loggedInUser.bots}
          data
          isCreator
        />
      }
    </MeasurementCreate.Mutation>,
};