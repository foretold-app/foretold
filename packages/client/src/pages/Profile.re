[@bs.config {jsx: 3}];

module FormConfig = {
  /* Define the form state */
  type state = {
    name: string,
    email: string,
    picture: string,
    description: string,
  };

  /* Defined the field types, used for validation and change handling */
  type fields = [ | `name | `email | `picture | `description];

  /* Now teach ReForm how to get and set your fields given the types */
  /* The syntax goes (field, getter, setter) */
  let lens = [
    (`name, s => s.name, (s, name) => {...s, name}),
    (`email, s => s.email, (s, email) => {...s, email}),
    (`picture, s => s.picture, (s, picture) => {...s, picture}),
    (
      `description,
      s => s.description,
      (s, description) => {...s, description},
    ),
  ];
};

module Form = ReFormNext.Make(FormConfig);

let testName = (str: string) => {
  let exp = () => [%re "/^[a-z0-9._]{0,30}$/i"];
  let res = exp() |> Js.Re.exec(str);
  switch (res) {
  | Some(_) => true
  | _ => false
  };
};

let withUserForm =
    (id, name, email, picture, description, mutation, innerComponentFn) =>
  Form.use(
    ~initialState={name, email, picture, description},
    ~onSubmit=
      values => {
        UserUpdate.mutate(
          mutation,
          values.state.values.name,
          values.state.values.email,
          values.state.values.picture,
          values.state.values.description,
          id,
        );
        None;
      },
    ~schema=
      Form.Validation.Schema([|
        Custom(
          Name,
          values => testName(values.name) ? Valid : Error(Lang.wrongName),
        ),
        Custom(Picture, _ => Valid),
        Custom(Description, _ => Valid),
      |]),
    (),
  )
  |> innerComponentFn;

let formFields = (state: Form.state, handleChange, getFieldState, submit) => {
  let onSubmit = () => submit(Form.Submit);

  let stateName = getFieldState(Form.Field(Name));
  let statePicture = getFieldState(Form.Field(Picture));
  let stateDescription = getFieldState(Form.Field(Description));
  let stateForm = state.formState;

  let error = state =>
    switch (state) {
    | ReFormNext.Error(s) => <AntdAlert message=s type_="warning" />
    | _ => <Null />
    };

  let isFormValid =
    switch (stateName, statePicture, stateDescription) {
    | (ReFormNext.Error(_), _, _) => false
    | (_, ReFormNext.Error(_), _) => false
    | (_, _, ReFormNext.Error(_)) => false
    | _ => true
    };

  let isFormDirty =
    switch (stateForm) {
    | Form.Dirty => true
    | _ => false
    };

  let isEnabled = isFormValid && isFormDirty;

  <Antd.Form onSubmit={_e => onSubmit()}>
    <Antd.Form.Item label={"Username" |> Utils.ste}>
      <Antd.Input
        value={state.values.name}
        onChange={ReForm.Helpers.handleDomFormChange(e => {
          handleChange(`name, e);
          ();
        })}
      />
      {error(stateName)}
    </Antd.Form.Item>
    <Antd.Form.Item label={"Description" |> Utils.ste}>
      <Antd.Input
        value={state.values.description}
        onChange={ReForm.Helpers.handleDomFormChange(e => {
          handleChange(`description, e);
          ();
        })}
      />
    </Antd.Form.Item>
    <Antd.Form.Item label={"Email" |> Utils.ste}>
      <Antd.Input
        value={state.values.email}
        disabled=true
        onChange={ReForm.Helpers.handleDomFormChange(e => {
          handleChange(`email, e);
          ();
        })}
      />
    </Antd.Form.Item>
    <Antd.Form.Item label={"Picture URL" |> Utils.ste}>
      <Antd.Input
        value={state.values.picture}
        onChange={ReForm.Helpers.handleDomFormChange(e => {
          handleChange(`picture, e);
          ();
        })}
      />
    </Antd.Form.Item>
    <Antd.Form.Item>
      <Antd.Button
        _type=`primary onClick={_ => onSubmit()} disabled={!isEnabled}>
        {"Submit" |> Utils.ste}
      </Antd.Button>
    </Antd.Form.Item>
  </Antd.Form>;
};

module CMutationForm =
  MutationForm.Make({
    type queryType = UserUpdate.Query.t;
  });

[@react.component]
let make = (~loggedUser: Types.user) => {
  <SLayout head={SLayout.Header.textDiv("Edit Profile Information")}>
    <FC.PageCard.BodyPadding>
      <UserUpdate.Mutation>
        ...{(mutation, data) => {
          let agent = loggedUser.agent;
          let id = loggedUser.id;
          let email = loggedUser.email |> E.O.default("");
          let picture = loggedUser.picture |> E.O.default("");
          let description = loggedUser.description |> E.O.default("");
          let name =
            agent
            |> E.O.bind(_, (r: Types.agent) => r.name)
            |> E.O.default("");

          withUserForm(
            id,
            name,
            email,
            picture,
            description,
            mutation,
            ({handleChange, state, getFieldState}: Form.api) =>
            CMutationForm.showWithLoading(
              ~result=data.result,
              ~form=formFields(state, handleChange, getFieldState),
              (),
            )
          );
        }}
      </UserUpdate.Mutation>
    </FC.PageCard.BodyPadding>
  </SLayout>;
};