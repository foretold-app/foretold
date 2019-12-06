[@bs.config {jsx: 3}];

open Antd;
open BsReform;

module FormConfig = [%lenses type state = {email: string}];

module Form = ReForm.Make(FormConfig);

let withForm = (channelId, email, mutation) =>
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
  );

let fields = (reform: Form.api) => {
  //  let stateEmail = getFieldState(Form.Field(Email));
  //
  //  let error = state =>
  //    switch (state) {
  //    | ReFormNext.Error(s) => <AntdAlert message=s type_="warning" />
  //    | _ => <Null />
  //    };
  //
  //  let isFormValid =
  //    switch (stateEmail) {
  //    | ReFormNext.Valid => true
  //    | ReFormNext.Pristine => false
  //    };
  //    | _ => false
  //        disabled={!isFormValid}
  <Antd.Form
    onSubmit={event => {
      ReactEvent.Synthetic.preventDefault(event);
      reform.submit();
    }}>
    <Form.Field
      field=FormConfig.Email
      render={({handleChange, error, value}) =>
        <Antd.Form.Item label={"Email*" |> Utils.ste}>
          <AntdInput
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
      <Button
        _type=`primary
        onClick={event => {
          ReactEvent.Synthetic.preventDefault(event);
          reform.submit();
        }}
        icon=Antd.IconName.usergroupAdd>
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
      {InvitationCreate.withMutation((mutation, data) => {
         let reform = withForm(channelId, "", mutation);
         let form = fields(reform);

         let onSuccess = _ =>
           <>
             <AntdAlert message=Lang.memberInvited type_="success" />
             form
           </>;

         <Form.Provider value=reform>
           {CMutationForm.showWithLoading2(
              ~result=data.result,
              ~form,
              ~onSuccess,
              (),
            )}
         </Form.Provider>;
       })}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};