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
  let query = Queries.GetUser.make(~auth0Id, ());
  Queries.GetUserQuery.make(~variables=query##variables, ({result}) =>
    result
    |> apolloResponseToResult
    <$> innerComponentFn
    |> Rationale.Result.result(idd, idd)
  )
  |> ReasonReact.element;
};

let withUserMutation = innerComponentFn =>
  EditUserMutation.make(~onCompleted=e => Js.log("HI"), innerComponentFn)
  |> ReasonReact.element;

let withUserForm = (id, name, mutation, innerComponentFn) =>
  Form.make(
    ~onSubmit=({values}) => mutate(mutation, values, id),
    ~initialState={name: name},
    ~schema=[(`name, Custom(_ => None))],
    innerComponentFn,
  )
  |> ReasonReact.element;

/* Belt.Option.map(e##user, r => r##name) */
let make = (~auth0Id, _children) => {
  ...component,
  render: _ =>
    withUserQuery(auth0Id, userQuery =>
      withUserMutation((mutation, _) => {
        let id =
          Belt.Option.map(userQuery##user, r => r##id) |> Option.default("");
        let name =
          Belt.Option.map(userQuery##user, r => r##name) |> Option.default("");
        withUserForm(
          id, name, mutation, ({handleSubmit, handleChange, form, _}) =>
          <form onSubmit=(ReForm.Helpers.handleDomFormSubmit(handleSubmit))>
            <h2> ("Edit Profile" |> ste) </h2>
            <Input
              value=form.values.name
              onChange=(
                ReForm.Helpers.handleDomFormChange(handleChange(`name))
              )
            />
            <Button _type=`primary onClick=(_ => handleSubmit())>
              ("Submit" |> ste)
            </Button>
          </form>
        );
      })
    ),
};