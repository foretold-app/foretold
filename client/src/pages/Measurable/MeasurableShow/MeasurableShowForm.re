let component = ReasonReact.statelessComponent("MeasurableShowForm");

let make = (~measurableId: string, ~isCreator: bool, _children) => {
  ...component,
  render: _ =>
    Queries.CreateMeasurementMutation.Mutation.make(
      ~onCompleted=_ => Js.log("Request submitted"),
      (mutation, data) =>
        <CdfInput
          onSubmit={
            ((value, competitorType, description)) =>
              Queries.CreateMeasurementMutation.mutate(
                mutation,
                measurableId,
                value,
                competitorType,
                description,
              )
          }
          data
          isCreator
        />,
    )
    |> ReasonReact.element,
};