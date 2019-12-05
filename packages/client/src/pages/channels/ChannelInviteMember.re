[@bs.config {jsx: 3}];

open Antd;

module Config = {
  type field(_) =
    | Email: field(string);

  type state = {email: string};

  type fields = [ | `email];

  let get: type value. (state, field(value)) => value =
    (state, field) =>
      switch (field) {
      | Email => state.email
      };

  let set: type value. (state, field(value), value) => state =
    (state, field, value) =>
      switch (field) {
      | Email => {...state, email: value}
      };
};

module Form = ReFormNext.Make(Config);

let withForm = (channelId, email, mutation, innerComponentFn) =>
  Form.use(
    ~initialState={email: email},
    ~onSubmitFail=ignore,
    ~onSubmit=
      values => {
        InvitationCreate.mutate(
          mutation,
          values.state.values.email,
          channelId,
        );
        None;
      },
    ~schema={
      Form.Validation.Schema([|Email(Email)|]);
    },
    (),
  )
  |> innerComponentFn;

let fields = (form: Form.state, handleChange, onSubmit, getFieldState) => {
  let stateEmail = getFieldState(Form.Field(Email));

  let error = state =>
    switch (state) {
    | ReFormNext.Error(s) => <AntdAlert message=s type_="warning" />
    | _ => <Null />
    };

  let isFormValid =
    switch (stateEmail) {
    | ReFormNext.Valid => true
    | ReFormNext.Pristine => false
    | _ => false
    };

  <Antd.Form onSubmit={e => onSubmit()}>
    <Antd.Form.Item label={"Email*" |> Utils.ste}>
      <AntdInput
        value={form.values.email}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          handleChange(Email, e)
        )}
      />
      {error(stateEmail)}
    </Antd.Form.Item>
    <Antd.Form.Item>
      <Button
        _type=`primary
        onClick={_ => onSubmit()}
        icon=Antd.IconName.usergroupAdd
        disabled={!isFormValid}>
        {"Email an Invitation" |> Utils.ste}
      </Button>
    </Antd.Form.Item>
  </Antd.Form>;
};

module CMutationForm =
  MutationForm.Make({
    type queryType = InvitationCreate.Query.t;
  });

[@react.component]
let make = (~channelId: string) => {
  <SLayout head={SLayout.Header.textDiv("Invite Member")}>
    <FC.PageCard.BodyPadding>
      {InvitationCreate.withMutation((mutation, data) =>
         withForm(
           channelId,
           "",
           mutation,
           ({handleChange, state, getFieldState, submit}: Form.api) => {
             let form =
               fields(state, handleChange, () => submit(), getFieldState);

             let onSuccess = _ =>
               <>
                 <AntdAlert message=Lang.memberInvited type_="success" />
                 form
               </>;

             CMutationForm.showWithLoading2(
               ~result=data.result,
               ~form,
               ~onSuccess,
               (),
             );
           },
         )
       )}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};