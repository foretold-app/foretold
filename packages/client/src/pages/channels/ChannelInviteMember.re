open Antd;

module Config = {
  type field(_) =
    | Email: field(string);

  type state = {email: string};

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
  <Form
    initialState={email: email}
    onSubmit={values =>
      InvitationCreate.mutate(mutation, values.state.values.email, channelId)
    }
    schema={Form.Validation.Schema([|Email(Email)|])}>
    ...innerComponentFn
  </Form>;

let fields = (form: Form.state, send, onSubmit, getFieldState) => {
  let stateEmail: Form.fieldState = getFieldState(Form.Field(Email));

  let error = state =>
    switch (state) {
    | Form.Error(s) => <AntdAlert message=s type_="warning" />
    | _ => ReasonReact.null
    };

  let isFormValid =
    switch (stateEmail) {
    | Form.Valid => true
    | Form.Pristine => false
    | _ => false
    };

  <Antd.Form onSubmit={e => onSubmit()}>
    <Antd.Form.Item>
      {"E-mail*:" |> Utils.ste |> E.React.inH3}
      <AntdInput
        value={form.values.email}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          send(Form.FieldChangeValue(Email, e))
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
        {"Submit" |> Utils.ste}
      </Button>
    </Antd.Form.Item>
  </Antd.Form>;
};

module CMutationForm =
  MutationForm.Make({
    type queryType = InvitationCreate.Query.t;
  });

let component = ReasonReact.statelessComponent("ChannelInviteMember");
let make = (~channelId: string, ~loggedInUser: Types.user, _children) => {
  ...component,
  render: _ =>
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Invite Member"),
      ~body=
        <Fc.FC.PageCard.BodyPadding>
          {InvitationCreate.withMutation((mutation, data) =>
             withForm(
               channelId,
               "",
               mutation,
               ({send, state, getFieldState}) => {
                 let form =
                   fields(
                     state,
                     send,
                     () => send(Form.Submit),
                     getFieldState,
                   );

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
        </Fc.FC.PageCard.BodyPadding>,
    )
    |> SLayout.FullPage.makeWithEl,
};