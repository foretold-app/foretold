[@bs.config {jsx: 3}];

open Antd;

module FormConfig = {
  /* Define the form state */
  type state = {
    stopAllEmails: bool,
    enableExperimentalFeatures: bool,
  };

  /* Defined the field types, used for validation and change handling */
  type fields = [ | `stopAllEmails | `enableExperimentalFeatures];

  /* Now teach ReForm how to get and set your fields given the types */
  /* The syntax goes (field, getter, setter) */
  let lens = [
    (
      `stopAllEmails,
      s => s.stopAllEmails,
      (s, stopAllEmails) => {...s, stopAllEmails},
    ),
    (
      `enableExperimentalFeatures,
      s => s.enableExperimentalFeatures,
      (s, enableExperimentalFeatures) => {...s, enableExperimentalFeatures},
    ),
  ];
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
  Form.use(
    ~initialState={stopAllEmails, enableExperimentalFeatures},
    ~onSubmit=
      values => {
        PreferenceUpdate.mutate(
          mutation,
          values.state.values.stopAllEmails,
          values.state.values.enableExperimentalFeatures,
          id,
        );
        None;
      },
    ~schema=Form.Validation.Schema([||]),
    (),
  )
  |> innerComponentFn;

let formFields = (form: Form.state, handleChange, onSubmit) =>
  <Antd.Form onSubmit={_e => onSubmit()}>
    <Antd.Form.Item label={"Do not send me emails" |> Utils.ste}>
      <AntdSwitch
        checked={form.values.stopAllEmails}
        onChange={e => handleChange(`stopAllEmails, e)}
      />
    </Antd.Form.Item>
    <Antd.Form.Item label={"Enable experimental features" |> Utils.ste}>
      <AntdSwitch
        checked={form.values.enableExperimentalFeatures}
        onChange={e => handleChange(`enableExperimentalFeatures, e)}
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
           ({handleChange, state}: Form.api) =>
           CMutationForm.showWithLoading(
             ~result=data.result,
             ~form=formFields(state, handleChange, () => send(Form.Submit)),
             (),
           )
         );
       })}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};