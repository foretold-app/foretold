open Antd;

module FormConfig = {
  type field(_) =
    | Name: field(string)
    | Email: field(string)
    | Picture: field(string)
    | Description: field(string);

  type state = {
    name: string,
    email: string,
    picture: string,
    description: string,
  };

  let get: type value. (state, field(value)) => value =
    (state, field) =>
      switch (field) {
      | Name => state.name
      | Email => state.email
      | Picture => state.picture
      | Description => state.description
      };

  let set: type value. (state, field(value), value) => state =
    (state, field, value) =>
      switch (field) {
      | Name => {...state, name: value}
      | Email => {...state, email: value}
      | Picture => {...state, picture: value}
      | Description => {...state, description: value}
      };
};

module Form = ReFormNext.Make(FormConfig);

let component = ReasonReact.statelessComponent("Profile");

let withUserForm =
    (id, name, email, picture, description, mutation, innerComponentFn) =>
  Form.make(
    ~initialState={name, email, picture, description},
    ~onSubmit=
      values =>
        UserUpdate.mutate(
          mutation,
          values.state.values.name,
          values.state.values.email,
          values.state.values.picture,
          values.state.values.description,
          id,
        ),
    ~schema=Form.Validation.Schema([||]),
    innerComponentFn,
  )
  |> E.React.el;

let formFields = (form: Form.state, send, onSubmit) =>
  <Antd.Form onSubmit={e => onSubmit()}>
    <Antd.Form.Item>
      {"Username" |> Utils.ste |> E.React.inH3}
      <Input
        value={form.values.name}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          send(Form.FieldChangeValue(Name, e))
        )}
      />
    </Antd.Form.Item>
    <Antd.Form.Item>
      {"Description" |> Utils.ste |> E.React.inH3}
      <Input
        value={form.values.description}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          send(Form.FieldChangeValue(Description, e))
        )}
      />
    </Antd.Form.Item>
    <Antd.Form.Item>
      {"E-mail" |> Utils.ste |> E.React.inH3}
      <AntdInput
        value={form.values.email}
        disabled=true
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          send(Form.FieldChangeValue(Email, e))
        )}
      />
    </Antd.Form.Item>
    <Antd.Form.Item>
      {"Picture URL" |> Utils.ste |> E.React.inH3}
      <Input
        value={form.values.picture}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          send(Form.FieldChangeValue(Picture, e))
        )}
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
    type queryType = UserUpdate.EditUser.t;
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
          {UserUpdate.withUserMutation((mutation, data) => {
             let agent = loggedInUser.agent;
             let id = loggedInUser.id;
             let email = loggedInUser.email |> E.O.default("");
             let picture = loggedInUser.picture |> E.O.default("");
             let description = loggedInUser.description |> E.O.default("");
             let name =
               agent
               |> E.O.bind(_, (r: Primary.Agent.t) => r.name)
               |> E.O.toExn("The logged in user needs an ID!");

             withUserForm(
               id,
               name,
               email,
               picture,
               description,
               mutation,
               ({send, state}) =>
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