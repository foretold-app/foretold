[@bs.config {jsx: 3}];

open BsReform;

module FormConfig = [%lenses
  type state = {
    name: string,
    email: string,
    picture: string,
    description: string,
  }
];

module Form = ReForm.Make(FormConfig);

let testName = (str: string) => {
  let exp = () => [%re "/^[a-z0-9._]{0,30}$/i"];
  let res = exp() |> Js.Re.exec(str);
  switch (res) {
  | Some(_) => true
  | _ => false
  };
};

let withUserForm = (id, name, email, picture, description, mutation) =>
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
        //        Custom(
        //          Name,
        //          values =>
        //            testName(values.name)
        //              ? ReForm.Valid : ReForm.Error(Lang.wrongName),
        //        ),
        //        Custom(Picture, _ => ReForm.Valid),
        //        Custom(Description, _ => ReForm.Valid),
      |]),
    (),
  );

let formFields = (reform: Form.api) => {
  //  let stateName = getFieldState(Form.Field(Name));
  //  let statePicture = getFieldState(Form.Field(Picture));
  //  let stateDescription = getFieldState(Form.Field(Description));
  //  let stateForm = state.formState;
  //
  //  let error = state =>
  //    switch (state) {
  //    | ReFormNext.Error(s) => <AntdAlert message=s type_="warning" />
  //    | _ => <Null />
  //    };
  //
  //  let isFormValid =
  //    switch (stateName, statePicture, stateDescription) {
  //    | (ReFormNext.Error(_), _, _) => false
  //    | (_, ReFormNext.Error(_), _) => false
  //    | (_, _, ReFormNext.Error(_)) => false
  //    | _ => true
  //    };
  //
  //  let isFormDirty =
  //    switch (stateForm) {
  //    | Form.Dirty => true
  //    | _ => false
  //    };
  //  let isEnabled = isFormValid && isFormDirty;
  <Antd.Form
    onSubmit={event => {
      ReactEvent.Synthetic.preventDefault(event);
      reform.submit();
    }}>
    <Form.Field
      field=FormConfig.Name
      render={({handleChange, error, value}) =>
        <Antd.Form.Item label={"Username" |> Utils.ste}>
          <Antd.Input
            value
            onChange={event =>
              ReactEvent.Form.target(event)##value |> handleChange
            }
          />
          {error->Belt.Option.getWithDefault("")->React.string}
        </Antd.Form.Item>
      }
    />
    <Form.Field
      field=FormConfig.Description
      render={({handleChange, error, value}) =>
        <Antd.Form.Item label={"Description" |> Utils.ste}>
          <Antd.Input
            value
            onChange={event =>
              ReactEvent.Form.target(event)##value |> handleChange
            }
          />
          {error->Belt.Option.getWithDefault("")->React.string}
        </Antd.Form.Item>
      }
    />
    <Form.Field
      field=FormConfig.Email
      render={({handleChange, error, value}) =>
        <Antd.Form.Item label={"Email" |> Utils.ste}>
          <Antd.Input
            value
            onChange={event =>
              ReactEvent.Form.target(event)##value |> handleChange
            }
          />
          {error->Belt.Option.getWithDefault("")->React.string}
        </Antd.Form.Item>
      }
    />
    <Form.Field
      field=FormConfig.Picture
      render={({handleChange, error, value}) =>
        <Antd.Form.Item label={"Picture URL" |> Utils.ste}>
          <Antd.Input
            value
            onChange={event =>
              ReactEvent.Form.target(event)##value |> handleChange
            }
          />
          {error->Belt.Option.getWithDefault("")->React.string}
        </Antd.Form.Item>
      }
    />
    <Antd.Form.Item>
      <Antd.Button
        _type=`primary
        onClick={event => {
          ReactEvent.Synthetic.preventDefault(event);
          reform.submit();
        }}>
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

          let reform =
            withUserForm(id, name, email, picture, description, mutation);

          <Form.Provider value=reform>
            {CMutationForm.showWithLoading(
               ~result=data.result,
               ~form=formFields(reform),
               (),
             )}
          </Form.Provider>;
        }}
      </UserUpdate.Mutation>
    </FC.PageCard.BodyPadding>
  </SLayout>;
};