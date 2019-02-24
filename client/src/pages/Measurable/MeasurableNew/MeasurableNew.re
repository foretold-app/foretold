open Utils;
open Rationale;
open Rationale.Option;
open Result.Infix;
open Rationale.Function.Infix;
open Antd;
open MomentRe;

let ste = ReasonReact.string;

module CreateMeasurableMutation = {
  module GraphQL = [%graphql
    {|
             mutation createMeasurable($name: String!, $description: String!, $valueType:valueType!, $expectedResolutionDate:Date, $resolutionEndpoint: String!, $descriptionEntity: String!) {
                 createMeasurable(name: $name, description: $description, valueType: $valueType, expectedResolutionDate: $expectedResolutionDate, resolutionEndpoint: $resolutionEndpoint, descriptionEntity: $descriptionEntity) {
                   id
                 }
             }
     |}
  ];

  module Mutation = ReasonApollo.CreateMutation(GraphQL);
};

module SignUpParams = {
  type state = {
    name: string,
    description: string,
    descriptionEntity: string,
    valueType: string,
    expectedResolutionDate: string,
    resolutionEndpoint: string,
  };
  type fields = [
    | `name
    | `valueType
    | `description
    | `descriptionEntity
    | `expectedResolutionDate
    | `resolutionEndpoint
  ];
  let lens = [
    (`name, s => s.name, (s, name) => {...s, name}),
    (
      `description,
      s => s.description,
      (s, description) => {...s, description},
    ),
    (
      `descriptionEntity,
      s => s.descriptionEntity,
      (s, descriptionEntity) => {...s, descriptionEntity},
    ),
    (`valueType, s => s.valueType, (s, valueType) => {...s, valueType}),
    (
      `expectedResolutionDate,
      s => s.expectedResolutionDate,
      (s, expectedResolutionDate) => {...s, expectedResolutionDate},
    ),
    (
      `resolutionEndpoint,
      s => s.resolutionEndpoint,
      (s, resolutionEndpoint) => {...s, resolutionEndpoint},
    ),
  ];
};

module SignUpForm = ReForm.Create(SignUpParams);

let mutate =
    (
      mutation: CreateMeasurableMutation.Mutation.apolloMutation,
      values: SignUpForm.values,
    ) => {
  let mutate =
    CreateMeasurableMutation.GraphQL.make(
      ~name=values.name,
      ~description=values.description,
      ~expectedResolutionDate=values.expectedResolutionDate |> Js.Json.string,
      ~resolutionEndpoint=values.resolutionEndpoint,
      ~descriptionEntity=values.descriptionEntity,
      ~valueType=
        switch (values.valueType) {
        | "float" => `FLOAT
        | "percentage" => `PERCENTAGE
        | "date" => `DATE
        | _ => `FLOAT
        },
      (),
    );
  mutation(~variables=mutate##variables, ()) |> ignore;
};

let formatDate = Moment.format("MMM DD, YYYY HH:MM:SS");
let component = ReasonReact.statelessComponent("Measurables");

let showForm = (~form: SignUpForm.state, ~handleSubmit, ~handleChange) =>
  <form onSubmit={ReForm.Helpers.handleDomFormSubmit(handleSubmit)}>
    <h2> {"Create a new Measurable" |> ste} </h2>
    <Form>
      <Form.Item>
        <h3> {"Name" |> ste} </h3>
        <Input
          value={form.values.name}
          onChange={ReForm.Helpers.handleDomFormChange(handleChange(`name))}
        />
      </Form.Item>
      <Form.Item>
        <h3> {"Entity" |> ste} </h3>
        <Input
          value={form.values.descriptionEntity}
          onChange={
            ReForm.Helpers.handleDomFormChange(
              handleChange(`descriptionEntity),
            )
          }
        />
      </Form.Item>
      <Form.Item>
        <h3> {"Description" |> ste} </h3>
        <Input
          value={form.values.description}
          onChange={
            ReForm.Helpers.handleDomFormChange(handleChange(`description))
          }
        />
      </Form.Item>
      <Form.Item>
        <h3> {"Resolution Endpoint (Optional)" |> ste} </h3>
        <p>
          {
            "If you enter an url that returns a number, this will be called when the resolution date occurs, and entered as a judgement value."
            |> ste
          }
        </p>
        <Input
          value={form.values.resolutionEndpoint}
          onChange={
            ReForm.Helpers.handleDomFormChange(
              handleChange(`resolutionEndpoint),
            )
          }
        />
      </Form.Item>
      <Form.Item>
        <h3> {"Expected Resolution Date" |> ste} </h3>
        <DatePicker
          value={
            form.values.expectedResolutionDate |> MomentRe.momentDefaultFormat
          }
          onChange={
            e => handleChange(`expectedResolutionDate, e |> formatDate)
          }
        />
      </Form.Item>
      <Form.Item>
        <Button _type=`primary onClick={_ => handleSubmit()}>
          {"Submit" |> ste}
        </Button>
      </Form.Item>
    </Form>
  </form>;

let make = _children => {
  ...component,
  render: _ =>
    CreateMeasurableMutation.Mutation.make(
      ~onCompleted=e => Js.log("HI"),
      (mutation, data) =>
        SignUpForm.make(
          ~onSubmit=({values}) => mutate(mutation, values),
          ~initialState={
            name: "",
            description: "",
            valueType: "float",
            descriptionEntity: "",
            expectedResolutionDate: MomentRe.momentNow() |> formatDate,
            resolutionEndpoint: "",
          },
          ~schema=[(`name, Custom(_ => None))],
          ({handleSubmit, handleChange, form, _}) =>
            switch (data.result) {
            | Loading => <div> {"Loading" |> ste} </div>
            | Error(e) =>
              <div>
                {"Error: " ++ e##message |> ste}
                {showForm(~form, ~handleSubmit, ~handleChange)}
              </div>
            | Data(data) =>
              data##createMeasurable
              |> Option.fmap(e => e##id)
              |> doIfSome(i => ReasonReact.Router.push("/measurables/" ++ i));
              <h2> {"Measurable successfully created" |> ste} </h2>;
            | NotCalled => showForm(~form, ~handleSubmit, ~handleChange)
            },
        )
        |> ReasonReact.element,
    )
    |> ReasonReact.element,
};