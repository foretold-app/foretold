open Antd;
open Foretold__GraphQL;

module EditPreference = [%graphql
  {|
    mutation preferenceUpdate(
        $id: String!
        $input: PreferenceUpdateInput!
    ) {
        preferenceUpdate(
            id: $id
            input: $input
        ) {
            id
        }
    }
 |}
];

module EditPreferenceMutation = ReasonApollo.CreateMutation(EditPreference);

module FormConfig = {
  type field(_) =
    | Emails: field(bool);

  type state = {emails: bool};

  let get: type value. (state, field(value)) => value =
    (state, field) =>
      switch (field) {
      | Emails => state.emails
      };

  let set: type value. (state, field(value), value) => state =
    (state, field, value) =>
      switch (field) {
      | Emails => {...state, emails: value}
      };
};

module Form = ReFormNext.Make(FormConfig);

let mutate =
    (
      mutation: EditPreferenceMutation.apolloMutation,
      emails: bool,
      id: string,
    ) => {
  let mutate =
    EditPreference.make(~id, ~input={"emails": Some(emails)}, ());
  mutation(~variables=mutate##variables, ~refetchQueries=[||], ()) |> ignore;
};

let component = ReasonReact.statelessComponent("Preference");

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

let withPreferenceMutation = innerComponentFn =>
  EditPreferenceMutation.make(
    ~onError=e => Js.log2("Graphql Error:", e),
    innerComponentFn,
  )
  |> E.React.el;

let withUserForm = (id, emails, mutation, innerComponentFn) =>
  Form.make(
    ~initialState={emails: emails},
    ~onSubmit=values => mutate(mutation, values.state.values.emails, id),
    ~schema=Form.Validation.Schema([||]),
    innerComponentFn,
  )
  |> E.React.el;

let formFields = (form: Form.state, send, onSubmit) =>
  <Antd.Form onSubmit={e => onSubmit()}>
    <Antd.Form.Item>
      {"Do not send me emails" |> Utils.ste |> E.React.inH3}
      <AntdSwitch
        checked={form.values.emails}
        onChange={e => send(Form.FieldChangeValue(Emails, true))}
      />
    </Antd.Form.Item>
    <Antd.Form.Item>
      <Button _type=`primary onClick={_ => onSubmit()}>
        {"Submit" |> Utils.ste}
      </Button>
    </Antd.Form.Item>
  </Antd.Form>;

module CMutationForm =
  MutationForm.Make({
    type queryType = EditPreference.t;
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
      ~head=SLayout.Header.textDiv("Preferences"),
      ~body=
        <FC.PageCard.BodyPadding>
          {withPreferenceMutation((mutation, data) => {
             let agent = loggedInUser.agent;
             let id = loggedInUser.id;
             let emails =
               agent
               |> E.O.bind(_, (r: Primary.Agent.t) => r.preference)
               |> E.O.bind(_, (r: Types.preference) => r.emails)
               |> E.O.default(true);

             withUserForm(id, emails, mutation, ({send, state}) =>
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