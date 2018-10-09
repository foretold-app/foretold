open Utils;
open Rationale;
open Rationale.Option;
open Result.Infix;
open Rationale.Function.Infix;
open Antd;

let ste = ReasonReact.string;

module CreateMeasurable = [%graphql
  {|
             mutation createMeasurable($name: String!, $description: String!, $valueType:valueType!) {
                 createMeasurable(name: $name, description: $description, valueType: $valueType) {
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
  };
  type fields = [ | `name | `valueType | `description];
  let lens = [
    (`name, s => s.name, (s, name) => {...s, name}),
    (
      `description,
      s => s.description,
      (s, description) => {...s, description},
    ),
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
      ~description=values.description,
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
          ~initialState={name: "", description: "", valueType: "float"},
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
                <Form.Item> <DatePicker /> </Form.Item>
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