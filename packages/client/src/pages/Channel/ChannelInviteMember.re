open Antd;

module FormConfig = {
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

module Form = ReFormNext.Make(FormConfig);

let withForm = (channelId, email, mutation, innerComponentFn) =>
  Form.make(
    ~initialState={email: email},
    ~onSubmit=
      values =>
        InvitationCreate.mutate(
          mutation,
          values.state.values.email,
          channelId,
        ),
    ~schema=Form.Validation.Schema([||]),
    innerComponentFn,
  )
  |> E.React.el;

let component = ReasonReact.statelessComponent("ChannelInviteMember");

let formFields = (form: Form.state, send, onSubmit) =>
  <Antd.Form onSubmit={e => onSubmit()}>
    <Antd.Form.Item>
      {"E-mail" |> Utils.ste |> E.React.inH3}
      <AntdInput
        value={form.values.email}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          send(Form.FieldChangeValue(Email, e))
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
    type queryType = InvitationCreate.Query.t;
  });

let make =
    (
      ~channelId: string,
      ~loggedInUser: Types.user,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ =>
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Invite Member"),
      ~body=
        <FC.PageCard.BodyPadding>
          {InvitationCreate.withMutation((mutation, data) =>
             withForm(channelId, "", mutation, ({send, state}) =>
               CMutationForm.showWithLoading(
                 ~result=data.result,
                 ~form=formFields(state, send, () => send(Form.Submit)),
                 (),
               )
             )
           )}
        </FC.PageCard.BodyPadding>,
    )
    |> layout,
};