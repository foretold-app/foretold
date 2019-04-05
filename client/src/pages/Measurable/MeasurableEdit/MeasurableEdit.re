open Utils;
open Foretold__GraphQL;
open Rationale.Function.Infix;

module WithEditMutation = {
  module GraphQL = [%graphql
    {|
             mutation measurableUpdate($id: String!, $name: String!, $labelCustom: String!, $expectedResolutionDate:Date, $resolutionEndpoint: String!, $labelSubject: String!,$labelProperty: String, $labelOnDate: Date) {
                 measurableUpdate(id: $id, name: $name, labelCustom: $labelCustom, expectedResolutionDate: $expectedResolutionDate, resolutionEndpoint: $resolutionEndpoint, labelSubject: $labelSubject,labelProperty: $labelProperty, labelOnDate: $labelOnDate) {
                   id
                 }
             }
     |}
  ];

  module Mutation = ReasonApollo.CreateMutation(GraphQL);

  let mutate =
      (
        mutation: Mutation.apolloMutation,
        id: string,
        name: string,
        labelCustom: string,
        expectedResolutionDate: string,
        resolutionEndpoint: string,
        labelSubject: string,
        labelOnDate: string,
        showDescriptionDate: string,
        labelProperty: string,
      ) => {
    let date = showDescriptionDate == "TRUE" ? labelOnDate : "";
    let m =
      GraphQL.make(
        ~id,
        ~name,
        ~labelCustom,
        ~labelProperty,
        ~labelOnDate=date |> Js.Json.string,
        ~expectedResolutionDate=expectedResolutionDate |> Js.Json.string,
        ~resolutionEndpoint,
        ~labelSubject,
        (),
      );
    mutation(~variables=m##variables, ~refetchQueries=[|"getAgent"|], ())
    |> ignore;
  };
};

module CMutationForm =
  MutationForm.Make({
    type queryType = WithEditMutation.GraphQL.t;
  });

let formCreation = (id, m) => {
  let measurable = Queries.Measurable.toMeasurable(m);
  WithEditMutation.Mutation.make((mutation, data) =>
    MeasurableForm.SignUpForm.make(
      ~onSubmit=
        ({values}) =>
          WithEditMutation.mutate(
            mutation,
            id,
            values.name,
            values.labelCustom,
            values.expectedResolutionDate,
            values.resolutionEndpoint,
            values.labelSubject,
            values.labelOnDate,
            values.showDescriptionDate,
            values.labelProperty,
          ),
      ~initialState={
        name: measurable.name,
        labelOnDate:
          measurable.labelOnDate
          |> E.O.default(MomentRe.momentNow())
          |> MeasurableForm.formatDate,
        showDescriptionDate:
          measurable.labelOnDate |> E.O.isSome |> (e => e ? "TRUE" : "FALSE"),
        labelSubject: measurable.labelSubject |> E.O.default(""),
        labelCustom: measurable.labelCustom |> E.O.default(""),
        expectedResolutionDate:
          measurable.expectedResolutionDate
          |> E.O.default(MomentRe.momentNow())
          |> MeasurableForm.formatDate,
        resolutionEndpoint: measurable.resolutionEndpoint |> E.O.default(""),
        showDescriptionProperty: measurable.name == "" ? "TRUE" : "FALSE",
        labelProperty: measurable.labelProperty |> E.O.default(""),
      },
      ~schema=[(`name, Custom(_ => None))],
      ({handleSubmit, handleChange, form, _}) =>
        CMutationForm.showWithLoading(
          ~result=data.result,
          ~form=MeasurableForm.showForm(~form, ~handleSubmit, ~handleChange),
          ~successMessage="Measurable updated successfully.",
          (),
        ),
    )
    |> E.React.el
  )
  |> E.React.el;
};

let component = ReasonReact.statelessComponent("MeasurableEdit");
let make = (~id: string, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _self =>
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("My Measurable"),
      ~body=Queries.Measurable.component(~id, m => formCreation(id, m)),
    )
    |> layout,
};