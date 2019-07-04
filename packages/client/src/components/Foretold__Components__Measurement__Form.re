open Foretold__GraphQL;

let component = ReasonReact.statelessComponent("MeasurementForm");

let make =
    (
      ~measurable: Primary.Measurable.t,
      ~measurableId: string,
      ~isCreator: bool,
      _children,
    ) => {
  ...component,
  render: _ =>
    Mutations.MeasurementCreate.Mutation.make(
      ~onCompleted=_ => Js.log("Request submitted"),
      (mutation, data) =>
        <CdfInput
          measurable
          onSubmit={((value, competitorType, description, valueText)) =>
            Mutations.MeasurementCreate.mutate(
              mutation,
              measurableId,
              value,
              competitorType,
              description,
              valueText,
            )
          }
          data
          isCreator
        />,
    )
    |> E.React.el,
};