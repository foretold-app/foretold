open Utils;
open Rationale;
open Rationale.Option;
open Result.Infix;
open Rationale.Function.Infix;
open Antd;
open MomentRe;

let ste = ReasonReact.string;

module CreateMeasurable = [%graphql
  {|
             mutation createMeasurable($name: String!, $description: String!, $valueType:valueType!, $expectedResolutionDate:Date) {
                 createMeasurable(name: $name, description: $description, valueType: $valueType, expectedResolutionDate: $expectedResolutionDate) {
                   id
                 }
             }
     |}
];

module CreateMeasurableMutation =
  ReasonApollo.CreateMutation(CreateMeasurable);

module SignUpParams = {
  type state = {
    name: string,
    description: string,
    valueType: string,
    expectedResolutionDate: string,
  };
  type fields = [
    | `name
    | `valueType
    | `description
    | `expectedResolutionDate
  ];
  let lens = [
    (`name, s => s.name, (s, name) => {...s, name}),
    (
      `description,
      s => s.description,
      (s, description) => {...s, description},
    ),
    (`valueType, s => s.valueType, (s, valueType) => {...s, valueType}),
    (
      `expectedResolutionDate,
      s => s.expectedResolutionDate,
      (s, expectedResolutionDate) => {...s, expectedResolutionDate},
    ),
  ];
};

module SignUpForm = ReForm.Create(SignUpParams);

let mutate =
    (
      mutation: CreateMeasurableMutation.apolloMutation,
      values: SignUpForm.values,
    ) => {
  let mutate =
    CreateMeasurable.make(
      ~name=values.name,
      ~description=values.description,
      ~expectedResolutionDate=values.expectedResolutionDate |> Js.Json.string,
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
let make = _children => {
  ...component,
  render: _ =>
    CreateMeasurableMutation.make(
      ~onCompleted=e => Js.log("HI"),
      (mutation, _) =>
        SignUpForm.make(
          ~onSubmit=({values}) => mutate(mutation, values),
          ~initialState={
            name: "",
            description: "",
            valueType: "float",
            expectedResolutionDate: MomentRe.momentNow() |> formatDate,
          },
          ~schema=[(`name, Custom(_ => None))],
          ({handleSubmit, handleChange, form, _}) =>
            <form onSubmit=(ReForm.Helpers.handleDomFormSubmit(handleSubmit))>
              <h2> ("Create a new Measurable" |> ste) </h2>
              <Form>
                <Form.Item>
                  <h3> ("Name" |> ste) </h3>
                  <Input
                    value=form.values.name
                    onChange=(
                      ReForm.Helpers.handleDomFormChange(handleChange(`name))
                    )
                  />
                </Form.Item>
                <Form.Item>
                  <h3> ("Description" |> ste) </h3>
                  <Input
                    value=form.values.description
                    onChange=(
                      ReForm.Helpers.handleDomFormChange(
                        handleChange(`description),
                      )
                    )
                  />
                </Form.Item>
                <Form.Item>
                  <h3> ("Expected Resolution Date" |> ste) </h3>
                  <DatePicker
                    value=(
                      form.values.expectedResolutionDate
                      |> MomentRe.momentDefaultFormat
                    )
                    onChange=(
                      e =>
                        handleChange(`expectedResolutionDate, e |> formatDate)
                    )
                  />
                </Form.Item>
                <Form.Item>
                  <h3> ("Value Type" |> ste) </h3>
                  <Select
                    value=form.values.valueType
                    onChange=(e => handleChange(`valueType, e) |> ignore)>
                    <Select.Option value="float">
                      ("Float" |> ste)
                    </Select.Option>
                    <Select.Option value="percentage">
                      ("Percentage" |> ste)
                    </Select.Option>
                    <Select.Option value="date">
                      ("Date" |> ste)
                    </Select.Option>
                  </Select>
                </Form.Item>
                <Form.Item>
                  <Button _type=`primary onClick=(_ => handleSubmit())>
                    ("Submit" |> ste)
                  </Button>
                </Form.Item>
              </Form>
            </form>,
        )
        |> ReasonReact.element,
    )
    |> ReasonReact.element,
};