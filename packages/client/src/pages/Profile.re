open Utils;
open Antd;
open Foretold__GraphQL;

let ste = ReasonReact.string;

module EditUser = [%graphql
  {|
             mutation userUpdate($id: String!, $input:UserUpdateInput!) {
                 userUpdate(id: $id, input: $input) {
                   id
                 }
             }
     |}
];

module EditUserMutation = ReasonApollo.CreateMutation(EditUser);

module FormConfig = {
  type field(_) =
    | Name: field(string);
  type state = {name: string};

  let get: type value. (state, field(value)) => value =
    (state, field) =>
      switch (field) {
      | Name => state.name
      };

  let set: type value. (state, field(value), value) => state =
    (state, field, value) =>
      switch (field) {
      | Name => {...state, name: value}
      };
};

/* ReFormNext */
module Form = ReFormNext.Make(FormConfig);

let mutate = (mutation: EditUserMutation.apolloMutation, values, id: string) => {
  let mutate = EditUser.make(~id, ~input={"name": values}, ());
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
    ~initialState={name: name},
    ~onSubmit=values => mutate(mutation, values.state.values.name, id),
    ~schema=Form.Validation.Schema([||]),
    innerComponentFn,
  )
  |> E.React.el;

let formFields = (form: Form.state, send, onSubmit) =>
  <Antd.Form onSubmit={e => onSubmit()}>
    <Antd.Form.Item>
      {"Username" |> ste |> E.React.inH3}
      <Input
        value={form.values.name}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          send(Form.FieldChangeValue(Name, e))
        )}
      />
    </Antd.Form.Item>
    <Antd.Form.Item>
      <Button _type=`primary onClick={_ => onSubmit()}>
        {"Submit" |> ste}
      </Button>
    </Antd.Form.Item>
  </Antd.Form>;

module CMutationForm =
  MutationForm.Make({
    type queryType = EditUser.t;
  });

let make =
    (
      ~loggedInUser: Primary.User.t,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ =>
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Edit Profile Information"),
      ~body=
        <FC.PageCard.BodyPadding>
          {withUserMutation((mutation, data) => {
             let agent = loggedInUser.agent;
             let id = loggedInUser.id;
             let name =
               agent
               |> E.O.bind(_, (r: Primary.Agent.t) => r.name)
               |> E.O.toExn("The logged in user needs an ID!");
             withUserForm(id, name, mutation, ({send, state}) =>
               CMutationForm.showWithLoading(
                 ~result=data.result,
                 ~form=formFields(state, send, () => send(Form.Submit)),
                 (),
               )
             );
           })}
        </FC.PageCard.BodyPadding>,
    )
    |> layout,
};