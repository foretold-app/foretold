open Utils;
open Antd;
open Foretold__GraphQL;

let ste = ReasonReact.string;

module EditUser = [%graphql
  {|
             mutation userUpdate($id: String!, $name:String!) {
                 userUpdate(id: $id, name: $name) {
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
  let query = Queries.User.Query.make(~auth0Id, ());
  Queries.User.QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    |> E.R.fmap(innerComponentFn)
    |> E.R.id
  )
  |> E.React.el;
};

let withUserMutation = innerComponentFn =>
  EditUserMutation.make(
    ~onError=e => Js.log2("Graphql Error:", e),
    innerComponentFn,
  )
  |> E.React.el;

let withUserForm = (id, name, mutation, innerComponentFn) =>
  Form.make(
    ~onSubmit=({values}) => mutate(mutation, values, id),
    ~initialState={name: name},
    ~schema=[(`name, Custom(_ => None))],
    innerComponentFn,
  )
  |> E.React.el;

let formFields = (form: Form.state, handleChange, handleSubmit: unit => unit) =>
  <form onSubmit={ReForm.Helpers.handleDomFormSubmit(handleSubmit)}>
    <Antd.Form>
      <Antd.Form.Item>
        {"Username" |> ste |> E.React.inH3}
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
    </Antd.Form>
  </form>;

module CMutationForm =
  MutationForm.Make({
    type queryType = EditUser.t;
  });

let make =
    (
      ~loggedInUser: Context.Primary.User.t,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ =>
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Edit Profile Information"),
      ~body=
        withUserMutation((mutation, data) => {
          let id = loggedInUser.id;
          let name = loggedInUser.name;
          withUserForm(
            id, name, mutation, ({handleSubmit, handleChange, form, _}) =>
            CMutationForm.showWithLoading(
              ~result=data.result,
              ~form=formFields(form, handleChange, handleSubmit),
              (),
            )
          );
        }),
    )
    |> layout,
};