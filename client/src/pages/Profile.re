open Utils;
open Rationale;
open Rationale.Option;
open Result.Infix;
open Rationale.Function.Infix;
open Antd;

let ste = ReasonReact.string;

module EditUser = [%graphql
  {|
             mutation editUser($id: String!, $name:String!) {
                 editUser(id: $id, name: $name) {
                   id
                 }
             }
     |}
];

module EditUserMutation = ReasonApollo.CreateMutation(EditUser);

module Params = {
  type state = {name: string};
  type fields = [ | `name];
  let lens = [(`name, s => s.name, (s, name) => {...s, name})];
};

module Form = ReForm.Create(Params);

let mutate =
    (
      mutation: EditUserMutation.apolloMutation,
      values: Form.values,
      id: string,
    ) => {
  let mutate = EditUser.make(~id, ~name=values.name, ());
  mutation(~variables=mutate##variables, ~refetchQueries=[|"user"|], ())
  |> ignore;
};

let component = ReasonReact.statelessComponent("Measurables");

let withUserQuery =
    (auth0Id, innerComponentFn: 'a => ReasonReact.reactElement) => {
  let query = Queries.GetUser.Query.make(~auth0Id, ());
  Queries.GetUser.QueryComponent.make(
    ~variables=query##variables, ({result}) =>
    result |> ApolloUtils.apolloResponseToResult <$> innerComponentFn |> E.R.id
  )
  |> ReasonReact.element;
};

let withUserMutation = innerComponentFn =>
  EditUserMutation.make(
    ~onError=e => Js.log2("Graphql Error:", e),
    innerComponentFn,
  )
  |> ReasonReact.element;

let withUserForm = (id, name, mutation, innerComponentFn) =>
  Form.make(
    ~onSubmit=({values}) => mutate(mutation, values, id),
    ~initialState={name: name},
    ~schema=[(`name, Custom(_ => None))],
    innerComponentFn,
  )
  |> ReasonReact.element;

let formFields = (form: Form.state, handleChange, handleSubmit: unit => unit) =>
  <div>
    <h2> {"Edit Profile" |> ste} </h2>
    <h3> {"Username" |> ste} </h3>
    <Input
      value={form.values.name}
      onChange={ReForm.Helpers.handleDomFormChange(handleChange(`name))}
    />
    <Button _type=`primary onClick={_ => handleSubmit()}>
      {"Submit" |> ste}
    </Button>
  </div>;

let make = (~auth0Id, _children) => {
  ...component,
  render: _ =>
    withUserQuery(auth0Id, userQuery =>
      withUserMutation((mutation, data) => {
        let id = E.O.fmap(r => r##id, userQuery##user) |> E.O.default("");
        let name =
          E.O.fmap(r => r##name, userQuery##user) |> E.O.default("");
        withUserForm(
          id, name, mutation, ({handleSubmit, handleChange, form, _}) =>
          <form onSubmit={ReForm.Helpers.handleDomFormSubmit(handleSubmit)}>
            {
              switch (data.result) {
              | Loading => <div> {"Loading" |> ste} </div>
              | Error(e) =>
                <div>
                  {"Error: " ++ e##message |> ste}
                  {formFields(form, handleChange, handleSubmit)}
                </div>
              | Data(_) =>
                <div>
                  {"Changes made successfully." |> ste}
                  {formFields(form, handleChange, handleSubmit)}
                </div>
              | NotCalled => formFields(form, handleChange, handleSubmit)
              }
            }
          </form>
        );
      })
    ),
};