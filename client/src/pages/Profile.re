open Utils;
open Rationale;
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
  let query = GetUser.Query.make(~auth0Id, ());
  GetUser.QueryComponent.make(~variables=query##variables, ({result}) =>
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
  <Antd.Form>
    <Antd.Form.Item>
      <h3> {"Username" |> ste} </h3>
      <Input
        value={form.values.name}
        onChange={ReForm.Helpers.handleDomFormChange(handleChange(`name))}
      />
    </Antd.Form.Item>
    <Antd.Form.Item>
      <Button _type=`primary onClick={_ => handleSubmit()}>
        {"Submit" |> ste}
      </Button>
    </Antd.Form.Item>
  </Antd.Form>;

let make = (~loggedInUser, _children) => {
  ...component,
  render: _ =>
    withUserMutation((mutation, data) => {
      let id =
        loggedInUser
        |> E.O.fmap((r: GetUser.user) => r.id)
        |> E.O.default("");
      let name =
        loggedInUser
        |> E.O.fmap((r: GetUser.user) => r.name)
        |> E.O.default("");
      withUserForm(
        id, name, mutation, ({handleSubmit, handleChange, form, _}) =>
        <div>
          <SLayout.Header>
            <h1> {"Edit Profile Information" |> ste} </h1>
          </SLayout.Header>
          <SLayout.MainSection>
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
          </SLayout.MainSection>
        </div>
      );
    }),
};