open BsReform;

module FormConfig = [%lenses type state = {email: string}];

module Form = ReForm.Make(FormConfig);

let schema =
  Form.Validation.Schema([|
    Email(Email),
    Custom(
      Email,
      values =>
        Js.String.length(values.email) > 64
          ? ReSchema.Error("Must be less than 64 characters.") : Valid,
    ),
    Custom(
      Email,
      values =>
        Js.String.length(values.email) < 3
          ? Error("Must be over 2 characters.") : Valid,
    ),
  |]);

module FormComponent = {
  [@react.component]
  let make =
      (
        ~reform: Form.api,
        ~result: ReasonApolloHooks.Mutation.controledVariantResult('a),
      ) => {
    let onSubmit = event => {
      ReactEvent.Synthetic.preventDefault(event);
      reform.submit();
    };

    <Form.Provider value=reform>
      {switch (result) {
       | Error(_error) => <Sorry />
       | Data(_) =>
         <Antd_Alert
           message={Lang.memberInvited |> Utils.ste}
           _type=`success
         />
       | _ =>
         <Antd.Form onSubmit>
           <Form.Field
             field=FormConfig.Email
             render={({handleChange, error, value}) =>
               <Antd.Form.Item label={"Email*" |> Utils.ste}>
                 <Antd.Input
                   value
                   onChange={Helpers.handleChange(handleChange)}
                 />
                 <Warning error />
               </Antd.Form.Item>
             }
           />
           <Antd.Form.Item>
             <Antd.Button
               _type=`primary onClick=onSubmit icon=Antd.IconName.usergroupAdd>
               {"Email an Invitation" |> Utils.ste}
             </Antd.Button>
           </Antd.Form.Item>
         </Antd.Form>
       }}
    </Form.Provider>;
  };
};

module Create = {
  [@react.component]
  let make = (~channelId) => {
    let (mutate, result, _) = InvitationCreate.Mutation.use();

    let reform =
      Form.use(
        ~validationStrategy=OnDemand,
        ~schema,
        ~onSubmit=
          ({state}) => {
            mutate(
              ~variables=
                InvitationCreate.Query.make(
                  ~input={
                    "email": state.values.email,
                    "channelId": channelId,
                  },
                  (),
                )##variables,
              (),
            )
            |> ignore;

            None;
          },
        ~initialState={email: ""},
        (),
      );

    <FormComponent reform result />;
  };
};

[@react.component]
let make = (~channelId: string) => {
  <SLayout head={<SLayout.TextDiv text="Invite Member" />}>
    <ForetoldComponents.PageCard.BodyPadding>
      <Create channelId />
    </ForetoldComponents.PageCard.BodyPadding>
  </SLayout>;
};