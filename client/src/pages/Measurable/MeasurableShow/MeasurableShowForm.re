open Foretold__GraphQL;

let component = ReasonReact.statelessComponent("MeasurableShowForm");

let make = (~measurableId: string, ~isCreator: bool, _children) => {
  ...component,
  render: _ =>
    Mutations.MeasurementCreate.Mutation.make(
      ~onCompleted=_ => Js.log("Request submitted"),
      (mutation, data) =>
        <CdfInput
          onSubmit={
            ((value, competitorType, description)) =>
              Mutations.MeasurementCreate.mutate(
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
    |> E.React.el,
};