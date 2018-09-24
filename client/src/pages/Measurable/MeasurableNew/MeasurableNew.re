open Utils;
open Rationale;
open Rationale.Option;
open Result.Infix;
open Rationale.Function.Infix;
open Antd;

let ste = ReasonReact.string;

module CreateMeasurable = [%graphql
  {|
             mutation createMeasurable($name: String!, $valueType:valueType!) {
                 createMeasurable(name: $name, valueType: $valueType) {
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
    valueType: string,
  };
  type fields = [ | `name | `valueType];
  let lens = [
    (`name, s => s.name, (s, name) => {...s, name}),
    (`valueType, s => s.valueType, (s, valueType) => {...s, valueType}),
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

let component = ReasonReact.statelessComponent("Measurables");
let make = _children => {
  ...component,
  render: _ =>
    CreateMeasurableMutation.make(
      ~onCompleted=e => Js.log("HI"),
      (mutation, _) =>
        SignUpForm.make(
          ~onSubmit=({values}) => mutate(mutation, values),
          ~initialState={name: "", valueType: "float"},
          ~schema=[(`name, Custom(_ => None))],
          ({handleSubmit, handleChange, form, _}) =>
            <form onSubmit=(ReForm.Helpers.handleDomFormSubmit(handleSubmit))>
              <h2> ("Create a new Measurable" |> ste) </h2>
              <Form>
                <Form.Item>
                  <Input
                    value=form.values.name
                    onChange=(
                      ReForm.Helpers.handleDomFormChange(handleChange(`name))
                    )
                  />
                </Form.Item>
                <Form.Item>
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