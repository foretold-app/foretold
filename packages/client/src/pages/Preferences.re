open Antd;

module FormConfig = {
  type field(_) =
    | StopAllEmails: field(bool);

  type state = {stopAllEmails: bool};

  let get: type value. (state, field(value)) => value =
    (state, field) =>
      switch (field) {
      | StopAllEmails => state.stopAllEmails
      };

  let set: type value. (state, field(value), value) => state =
    (state, field, value) =>
      switch (field) {
      | StopAllEmails => {...state, stopAllEmails: value}
      };
};

module Form = ReFormNext.Make(FormConfig);

let withUserForm = (id, stopAllEmails, mutation, innerComponentFn) =>
  Form.make(
    ~initialState={stopAllEmails: stopAllEmails},
    ~onSubmit=
      values =>
        PreferenceUpdate.mutate(
          mutation,
          values.state.values.stopAllEmails,
          id,
        ),
    ~schema=Form.Validation.Schema([||]),
    innerComponentFn,
  )
  |> E.React.el;

let component = ReasonReact.statelessComponent("Preference");

let formFields = (form: Form.state, send, onSubmit) =>
  <Antd.Form onSubmit={e => onSubmit()}>
    <Antd.Form.Item>
      {"Do not send me emails" |> Utils.ste |> E.React.inH3}
      <AntdSwitch
        checked={form.values.stopAllEmails}
        onChange={e => send(Form.FieldChangeValue(StopAllEmails, e))}
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
    type queryType = PreferenceUpdate.EditPreference.t;
  });

let make =
    (
      ~loggedInUser: Types.user,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ =>
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Preferences"),
      ~body=
        <FC.PageCard.BodyPadding>
          {PreferenceUpdate.withPreferenceMutation((mutation, data) => {
             let agent = loggedInUser.agent;
             let id =
               agent
               |> E.O.bind(_, (r: Types.agent) => r.preference)
               |> E.O.fmap((r: Types.preference) => r.id)
               |> E.O.toExn("The preference needs an ID!");
             let stopAllEmails =
               agent
               |> E.O.bind(_, (r: Types.agent) => r.preference)
               |> E.O.bind(_, (r: Types.preference) => r.stopAllEmails)
               |> E.O.default(true);

             withUserForm(id, stopAllEmails, mutation, ({send, state}) =>
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