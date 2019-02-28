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
             mutation createMeasurable($name: String!, $description: String!, $valueType:valueType!, $expectedResolutionDate:Date, $resolutionEndpoint: String!, $descriptionEntity: String!, $descriptionDate: Date, $channel: String) {
                 createMeasurable(name: $name, description: $description, valueType: $valueType, expectedResolutionDate: $expectedResolutionDate, resolutionEndpoint: $resolutionEndpoint, descriptionEntity: $descriptionEntity, descriptionDate: $descriptionDate, channel: $channel) {
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
    descriptionDate: string,
    valueType: string,
    expectedResolutionDate: string,
    resolutionEndpoint: string,
    showDescriptionDate: string,
  };
  type fields = [
    | `name
    | `valueType
    | `description
    | `descriptionEntity
    | `descriptionDate
    | `expectedResolutionDate
    | `resolutionEndpoint
    | `showDescriptionDate
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
    (
      `showDescriptionDate,
      s => s.showDescriptionDate,
      (s, showDescriptionDate) => {...s, showDescriptionDate},
    ),
    (`valueType, s => s.valueType, (s, valueType) => {...s, valueType}),
    (
      `descriptionDate,
      s => s.descriptionDate,
      (s, descriptionDate) => {...s, descriptionDate},
    ),
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
      channel: string,
    ) => {
  let mutate =
    values.showDescriptionDate == "TRUE" ?
      CreateMeasurableMutation.GraphQL.make(
        ~name=values.name,
        ~description=values.description,
        ~expectedResolutionDate=
          values.expectedResolutionDate |> Js.Json.string,
        ~resolutionEndpoint=values.resolutionEndpoint,
        ~descriptionEntity=values.descriptionEntity,
        ~descriptionDate=values.descriptionDate |> Js.Json.string,
        ~channel,
        ~valueType=
          switch (values.valueType) {
          | "float" => `FLOAT
          | "percentage" => `PERCENTAGE
          | "date" => `DATE
          | _ => `FLOAT
          },
        (),
      ) :
      CreateMeasurableMutation.GraphQL.make(
        ~name=values.name,
        ~description=values.description,
        ~expectedResolutionDate=
          values.expectedResolutionDate |> Js.Json.string,
        ~resolutionEndpoint=values.resolutionEndpoint,
        ~descriptionEntity=values.descriptionEntity,
        ~channel,
        ~valueType=
          switch (values.valueType) {
          | "float" => `FLOAT
          | "percentage" => `PERCENTAGE
          | "date" => `DATE
          | _ => `FLOAT
          },
        (),
      );
  mutation(
    ~variables=mutate##variables,
    ~refetchQueries=[|"getMeasurables"|],
    (),
  )
  |> ignore;
};

let formatDate = Moment.format("MMM DD, YYYY HH:MM:SS");
let component = ReasonReact.statelessComponent("Measurables");

let showForm = (~form: SignUpForm.state, ~handleSubmit, ~handleChange) =>
  <form onSubmit={ReForm.Helpers.handleDomFormSubmit(handleSubmit)}>
    <h2> {"Create a new Measurable" |> ste} </h2>
    <Form>
      <Form.Item>
        <h3> {"Relevant Entity (optional)" |> ste} </h3>
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
        <h3> {"Measurable Name" |> ste} </h3>
        <Input
          value={form.values.name}
          onChange={ReForm.Helpers.handleDomFormChange(handleChange(`name))}
        />
      </Form.Item>
      <Form.Item>
        <h3> {"Include a Specific Date in Name" |> ste} </h3>
        <AntdSwitch
          checked={form.values.showDescriptionDate == "TRUE"}
          onChange={
            e => handleChange(`showDescriptionDate, e ? "TRUE" : "FALSE")
          }
        />
      </Form.Item>
      {
        form.values.showDescriptionDate == "TRUE" ?
          <Form.Item>
            <h3> {"'On' Date" |> ste} </h3>
            <DatePicker
              value={
                form.values.descriptionDate |> MomentRe.momentDefaultFormat
              }
              onChange={e => handleChange(`descriptionDate, e |> formatDate)}
            />
          </Form.Item> :
          <div />
      }
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

let make = (~channel, _children) => {
  ...component,
  render: _ =>
    CreateMeasurableMutation.Mutation.make(
      ~onCompleted=e => Js.log("HI"),
      (mutation, data) =>
        SignUpForm.make(
          ~onSubmit=({values}) => mutate(mutation, values, channel),
          ~initialState={
            name: "",
            description: "",
            valueType: "float",
            descriptionEntity: "",
            expectedResolutionDate: MomentRe.momentNow() |> formatDate,
            descriptionDate: MomentRe.momentNow() |> formatDate,
            resolutionEndpoint: "",
            showDescriptionDate: "FALSE",
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
              |> doIfSome(i => ReasonReact.Router.push("/c/" ++ channel));
              <h2> {"Measurable successfully created" |> ste} </h2>;
            | NotCalled => showForm(~form, ~handleSubmit, ~handleChange)
            },
        )
        |> ReasonReact.element,
    )
    |> ReasonReact.element
    |> FillWithSidebar.make(~channel=Some(channel))
    |> ReasonReact.element,
};