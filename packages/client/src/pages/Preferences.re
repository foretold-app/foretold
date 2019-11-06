[@bs.config {jsx: 3}];

open Antd;

module FormConfig = {
  type field(_) =
    | StopAllEmails: field(bool)
    | EnableExperimentalFeatures: field(bool);

  type state = {
    stopAllEmails: bool,
    enableExperimentalFeatures: bool,
  };

  let get: type value. (state, field(value)) => value =
    (state, field) =>
      switch (field) {
      | StopAllEmails => state.stopAllEmails
      | EnableExperimentalFeatures => state.enableExperimentalFeatures
      };

  let set: type value. (state, field(value), value) => state =
    (state, field, value) =>
      switch (field) {
      | StopAllEmails => {...state, stopAllEmails: value}
      | EnableExperimentalFeatures => {
          ...state,
          enableExperimentalFeatures: value,
        }
      };
};

module Form = ReFormNext.Make(FormConfig);

let withUserForm =
    (
      id,
      stopAllEmails,
      enableExperimentalFeatures,
      mutation,
      innerComponentFn,
    ) =>
  Form.make(
    ~initialState={stopAllEmails, enableExperimentalFeatures},
    ~onSubmit=
      values =>
        PreferenceUpdate.mutate(
          mutation,
          values.state.values.stopAllEmails,
          values.state.values.enableExperimentalFeatures,
          id,
        ),
    ~schema=Form.Validation.Schema([||]),
    innerComponentFn,
  )
  |> E.React2.el;

let formFields = (form: Form.state, send, onSubmit) =>
  <Antd.Form onSubmit={_e => onSubmit()}>
    <Antd.Form.Item label={"Do not send me emails" |> Utils.ste}>
      <AntdSwitch
        checked={form.values.stopAllEmails}
        onChange={e => send(Form.FieldChangeValue(StopAllEmails, e))}
      />
    </Antd.Form.Item>
    <Antd.Form.Item label={"Enable experimental features" |> Utils.ste}>
      <AntdSwitch
        checked={form.values.enableExperimentalFeatures}
        onChange={e =>
          send(Form.FieldChangeValue(EnableExperimentalFeatures, e))
        }
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

[@react.component]
let make = (~loggedUser: Types.user) => {
  <SLayout head={SLayout.Header.textDiv("Preferences")}>
    <FC.PageCard.BodyPadding>
      {PreferenceUpdate.withPreferenceMutation((mutation, data) => {
         let agent = loggedUser.agent;
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
         let enableExperimentalFeatures =
           agent
           |> E.O.bind(_, (r: Types.agent) => r.preference)
           |> E.O.fmap((r: Types.preference) => r.enableExperimentalFeatures)
           |> E.O.default(true);

         withUserForm(
           id,
           stopAllEmails,
           enableExperimentalFeatures,
           mutation,
           ({send, state}) =>
           CMutationForm.showWithLoading(
             ~result=data.result,
             ~form=formFields(state, send, () => send(Form.Submit)),
             (),
           )
         );
       })}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};