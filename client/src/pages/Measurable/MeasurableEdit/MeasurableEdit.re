open Utils;
open Rationale;
open MomentRe;
open Antd;

module WithEditMutation = {
  module GraphQL = [%graphql
    {|
             mutation editMeasurable($id: String!, $name: String!, $description: String!, $expectedResolutionDate:Date, $resolutionEndpoint: String!, $descriptionEntity: String!,$descriptionProperty: String, $descriptionDate: Date) {
                 editMeasurable(id: $id, name: $name, description: $description, expectedResolutionDate: $expectedResolutionDate, resolutionEndpoint: $resolutionEndpoint, descriptionEntity: $descriptionEntity,descriptionProperty: $descriptionProperty, descriptionDate: $descriptionDate) {
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
        description: string,
        expectedResolutionDate: string,
        resolutionEndpoint: string,
        descriptionEntity: string,
        descriptionDate: string,
        showDescriptionDate: string,
        descriptionProperty: string,
      ) => {
    let date = showDescriptionDate == "TRUE" ? descriptionDate : "";
    let m =
      GraphQL.make(
        ~id,
        ~name,
        ~description,
        ~descriptionProperty,
        ~descriptionDate=date |> Js.Json.string,
        ~expectedResolutionDate=expectedResolutionDate |> Js.Json.string,
        ~resolutionEndpoint,
        ~descriptionEntity,
        (),
      );
    mutation(~variables=m##variables, ~refetchQueries=[|"getAgent"|], ())
    |> ignore;
  };
};

let formCreation = (id, m) => {
  let measurable = GetMeasurable.toMeasurable(m);
  WithEditMutation.Mutation.make((mutation, data) =>
    MeasurableForm.SignUpForm.make(
      ~onSubmit=
        ({values}) =>
          WithEditMutation.mutate(
            mutation,
            id,
            values.name,
            values.description,
            values.expectedResolutionDate,
            values.resolutionEndpoint,
            values.descriptionEntity,
            values.descriptionDate,
            values.showDescriptionDate,
            values.descriptionProperty,
          ),
      ~initialState={
        name: measurable.name,
        descriptionDate:
          measurable.descriptionDate
          |> E.O.default(MomentRe.momentNow())
          |> MeasurableForm.formatDate,
        showDescriptionDate:
          measurable.descriptionDate
          |> E.O.isSome
          |> (e => e ? "TRUE" : "FALSE"),
        descriptionEntity: measurable.descriptionEntity |> E.O.default(""),
        description: measurable.description |> E.O.default(""),
        expectedResolutionDate:
          measurable.expectedResolutionDate
          |> E.O.default(MomentRe.momentNow())
          |> MeasurableForm.formatDate,
        resolutionEndpoint: measurable.resolutionEndpoint |> E.O.default(""),
        showDescriptionProperty: measurable.name == "" ? "TRUE" : "FALSE",
        descriptionProperty:
          measurable.descriptionProperty |> E.O.default(""),
      },
      ~schema=[(`name, Custom(_ => None))],
      ({handleSubmit, handleChange, form, _}) =>
        switch (data.result) {
        | Loading => "Loading" |> ste
        | Error(e) =>
          <div>
            {"Error: " ++ e##message |> ste}
            {MeasurableForm.showForm(~form, ~handleSubmit, ~handleChange)}
          </div>
        | Data(_) =>
          <div>
            <h3> {"Measurable successfully updated." |> ste} </h3>
            <div>
              {MeasurableForm.showForm(~form, ~handleSubmit, ~handleChange)}
            </div>
          </div>
        | NotCalled =>
          MeasurableForm.showForm(~form, ~handleSubmit, ~handleChange)
        },
    )
    |> ReasonReact.element
  )
  |> ReasonReact.element;
};

let component = ReasonReact.statelessComponent("MeasurableEdit");
let make = (~id: string, _children) => {
  ...component,
  render: _self =>
    <div>
      <SLayout.Header>
        {SLayout.Header.textDiv("Edit Measurable")}
      </SLayout.Header>
      <SLayout.MainSection>
        {GetMeasurable.component(~id, m => formCreation(id, m))}
      </SLayout.MainSection>
    </div>,
};