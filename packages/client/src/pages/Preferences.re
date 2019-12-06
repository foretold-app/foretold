[@bs.config {jsx: 3}];

open Antd;

open BsReform;

module FormConfig = [%lenses
  type state = {
    stopAllEmails: bool,
    enableExperimentalFeatures: bool,
  }
];

module Form = ReForm.Make(FormConfig);

let withUserForm = (id, stopAllEmails, enableExperimentalFeatures, mutation) =>
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
  );

let formFields = (reform: Form.api) =>
  <Antd.Form
    onSubmit={event => {
      ReactEvent.Synthetic.preventDefault(event);
      reform.submit();
    }}>
    <Form.Field
      field=FormConfig.StopAllEmails
      render={({handleChange, error, value}) =>
        <Antd.Form.Item label={"Do not send me emails" |> Utils.ste}>
          <AntdSwitch checked=value onChange={e => e |> handleChange} />
        </Antd.Form.Item>
      }
    />
    <Form.Field
      field=FormConfig.StopAllEmails
      render={({handleChange, error, value}) =>
        <Antd.Form.Item label={"Enable experimental features" |> Utils.ste}>
          <AntdSwitch checked=value onChange={e => e |> handleChange} />
        </Antd.Form.Item>
      }
    />
    <Antd.Form.Item>
      <Button
        _type=`primary
        onClick={event => {
          ReactEvent.Synthetic.preventDefault(event);
          reform.submit();
        }}>
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

         let reform =
           withUserForm(
             id,
             stopAllEmails,
             enableExperimentalFeatures,
             mutation,
           );

         <Form.Provider value=reform>
           {CMutationForm.showWithLoading(
              ~result=data.result,
              ~form=formFields(reform),
              (),
            )}
         </Form.Provider>;
       })}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};