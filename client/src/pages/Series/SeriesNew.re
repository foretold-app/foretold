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

module FormConfig = {
  type field(_) =
    | Subjects: field(string)
    | Properties: field(string);
  type state = {
    subjects: string,
    properties: string,
  };

  let get: type value. (state, field(value)) => value =
    (state, field) =>
      switch (field) {
      | Subjects => state.subjects
      | Properties => state.properties
      };

  let set: type value. (state, field(value), value) => state =
    (state, field, value) =>
      switch (field) {
      | Subjects => {...state, subjects: value}
      | Properties => {...state, properties: value}
      };
};

/* ReFormNext */
module Form = ReFormNext.Make(FormConfig);

let mutate = (mutation: EditUserMutation.apolloMutation, values, id: string) => {
  let mutate = EditUser.make(~id, ~name=values, ());
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

let withUserForm = (id, subjects, mutation, innerComponentFn) =>
  Form.make(
    ~initialState={subjects, properties: ""},
    ~onSubmit=values => (),
    ~schema=Form.Validation.Schema([||]),
    innerComponentFn,
  )
  |> E.React.el;

let formFields = (form: Form.state, send, onSubmit) =>
  <Antd.Form onSubmit={e => onSubmit()}>
    <Antd.Form.Item>
      {"Subjects" |> ste |> E.React.inH3}
      <Input.TextArea
        value={form.values.subjects}
        onChange={
          ReForm.Helpers.handleDomFormChange(e =>
            send(Form.FieldChangeValue(Subjects, e))
          )
        }
      />
    </Antd.Form.Item>
    <Antd.Form.Item>
      {"Properties" |> ste |> E.React.inH3}
      <Input.TextArea
        value={form.values.subjects}
        onChange={
          ReForm.Helpers.handleDomFormChange(e =>
            send(Form.FieldChangeValue(Subjects, e))
          )
        }
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
          let agent = loggedInUser.agent;
          let id = loggedInUser.id;
          let name =
            agent
            |> E.O.bind(_, (r: Context.Primary.Agent.t) => r.name)
            |> E.O.toExn("The logged in user needs an ID!");
          withUserForm(id, name, mutation, ({send, state}) =>
            CMutationForm.showWithLoading(
              ~result=data.result,
              ~form=formFields(state, send, () => send(Form.Submit)),
              (),
            )
          );
        }),
    )
    |> layout,
};