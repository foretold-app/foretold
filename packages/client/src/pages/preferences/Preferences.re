open Antd;

open BsReform;

module FormConfig = [%lenses
  type state = {
    stopAllEmails: bool,
    enableExperimentalFeatures: bool,
  }
];

module Form = ReForm.Make(FormConfig);

let schema = Form.Validation.Schema([||]);

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
       | Error(_error) => <p> {Lang.networkError |> Utils.ste} </p>
       | Data(_) => <p> {"Prefereces are saved." |> Utils.ste} </p>
       | _ =>
         <Antd.Form onSubmit>
           <Form.Field
             field=FormConfig.StopAllEmails
             render={({handleChange, value}) =>
               <Antd.Form.Item label={"Do not send me emails" |> Utils.ste}>
                 <AntdSwitch checked=value onChange={e => e |> handleChange} />
               </Antd.Form.Item>
             }
           />
           <Form.Field
             field=FormConfig.EnableExperimentalFeatures
             render={({handleChange, value}) =>
               <Antd.Form.Item
                 label={"Enable experimental features" |> Utils.ste}>
                 <AntdSwitch checked=value onChange={e => e |> handleChange} />
               </Antd.Form.Item>
             }
           />
           <Antd.Form.Item>
             {reform.state.formState == Submitting
                ? <Spin />
                : <Antd.Button _type=`primary onClick=onSubmit>
                    {"Submit" |> Utils.ste}
                  </Antd.Button>}
           </Antd.Form.Item>
         </Antd.Form>
       }}
    </Form.Provider>;
  };
};

module Edit = {
  [@react.component]
  let make = (~id, ~stopAllEmails, ~enableExperimentalFeatures) => {
    let (mutate, result, _) = PreferenceUpdate.Mutation.use();

    let reform =
      Form.use(
        ~validationStrategy=OnDemand,
        ~schema,
        ~onSubmit=
          ({state}) => {
            mutate(
              ~variables=
                PreferenceUpdate.Query.make(
                  ~id,
                  ~input={
                    "stopAllEmails": Some(state.values.stopAllEmails),
                    "enableExperimentalFeatures":
                      Some(state.values.enableExperimentalFeatures),
                  },
                  (),
                )##variables,
              ~refetchQueries=[|"user"|],
              (),
            )
            |> ignore;

            None;
          },
        ~initialState={stopAllEmails, enableExperimentalFeatures},
        (),
      );

    <FormComponent reform result />;
  };
};

[@react.component]
let make = (~loggedUser: Types.user) => {
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

  <SLayout head={SLayout.Header.textDiv("Preferences")}>
    <FC.PageCard.BodyPadding>
      <Edit id stopAllEmails enableExperimentalFeatures />
    </FC.PageCard.BodyPadding>
  </SLayout>;
};