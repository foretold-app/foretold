open BsReform;

module FormConfig = [%lenses
  type state = {
    name: string,
    description: string,
    isPublic: string,
    isArchived: string,
    requireVerification: string,
    knowledgeGraph: string,
  }
];

module Form = ReForm.Make(FormConfig);

type result('a) = ReasonApolloHooks.Mutation.controledVariantResult('a);

let schema =
  Form.Validation.Schema([|
    Custom(
      Name,
      values =>
        Js.String.length(values.name) > 64
          ? ReSchema.Error("Must be less than 64 characters.") : Valid,
    ),
    Custom(
      Name,
      values =>
        Js.String.length(values.name) < 3
          ? Error("Must be over 2 characters.") : Valid,
    ),
    Custom(
      KnowledgeGraph,
      values =>
        Js.String.length(values.knowledgeGraph) > 32 * 1024
          ? Error("Must be less the 32K.") : Valid,
    ),
    Custom(
      KnowledgeGraph,
      values =>
        switch (Toml.realParse(values.knowledgeGraph)) {
        | Ok(_) => Valid
        | Error(_) => Error("TOML Parsing Error")
        },
    ),
  |]);

let onSuccess = result => {
  switch (result) {
  | Some(channel) =>
    Utils.setTimeout(_ => Routing.Url.push(ChannelShow(channel##id)), 1000)
    |> ignore;
    ();
  | _ => ()
  };
};

module FormComponent = {
  [@react.component]
  let make = (~creating, ~reform: Form.api, ~result: result('a)) => {
    let context = React.useContext(Providers.app);

    let onSubmit = event => {
      ReactEvent.Synthetic.preventDefault(event);
      reform.submit();
    };

    <Form.Provider value=reform>
      {switch (result) {
       | Error(_error) => <Sorry />
       | Data(_) => <Spin />
       | _ =>
         <Antd.Form onSubmit>
           <Form.Field
             field=FormConfig.Name
             render={({handleChange, error, value}) =>
               <Antd.Form.Item label={"Name" |> Utils.ste}>
                 <Antd.Input
                   value
                   onChange={Helpers.handleChange(handleChange)}
                 />
                 <Warning error />
               </Antd.Form.Item>
             }
           />
           <Form.Field
             field=FormConfig.Description
             render={({handleChange, error, value}) =>
               <Antd.Form.Item
                 label={"Description" |> Utils.ste}
                 help={"Markdown supported" |> Utils.ste}>
                 <ForetoldComponents.ReAutosizeTextareaInput
                   value
                   className="ant-input"
                   minRows=4
                   onChange=handleChange
                 />
                 <Warning error />
               </Antd.Form.Item>
             }
           />
           <Experimental>
             <Form.Field
               field=FormConfig.KnowledgeGraph
               render={({handleChange, error, value}) =>
                 <Antd.Form.Item
                   label={"Knowledge graph" |> Utils.ste}
                   help={"Toml Supported" |> Utils.ste}>
                   <ForetoldComponents.ReAutosizeTextareaInput
                     value
                     className="ant-input"
                     minRows=4
                     onChange=handleChange
                   />
                   {switch (Toml.realParse(value)) {
                    | Ok(r) => Js.Json.stringify(r) |> Utils.ste
                    | Error(error) =>
                      <Antd_Alert message={error |> Utils.ste} _type=`error />
                    }}
                   <Warning error />
                 </Antd.Form.Item>
               }
             />
           </Experimental>
           <Form.Field
             field=FormConfig.IsPublic
             render={({handleChange, error, value}) =>
               <Antd.Form.Item label={"Community is public" |> Utils.ste}>
                 <Antd_Switch
                   checked={value == "TRUE"}
                   onChange={e => handleChange(e ? "TRUE" : "FALSE")}
                 />
                 <Warning error />
               </Antd.Form.Item>
             }
           />
           {E.React2.showIf(
              !creating,
              <Form.Field
                field=FormConfig.IsArchived
                render={({handleChange, error, value}) =>
                  <Antd.Form.Item label={"Archive community" |> Utils.ste}>
                    <Antd_Switch
                      checked={value == "TRUE"}
                      onChange={e => handleChange(e ? "TRUE" : "FALSE")}
                    />
                    <Warning error />
                  </Antd.Form.Item>
                }
              />,
            )}
           <Form.Field
             field=FormConfig.RequireVerification
             render={({handleChange, error, value}) =>
               <Antd.Form.Item
                 label={
                   "Restrict forecasting to verified members" |> Utils.ste
                 }>
                 <Antd_Switch
                   checked={value == "TRUE"}
                   onChange={e => handleChange(e ? "TRUE" : "FALSE")}
                 />
                 <Warning error />
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

module Create = {
  let onSuccess' = data => onSuccess(data##channelCreate);

  [@react.component]
  let make = () => {
    let (mutate, result, _) = ChannelCreate.Mutation.use();

    let reform =
      Form.use(
        ~validationStrategy=OnDemand,
        ~schema,
        ~onSubmit=
          ({state}) => {
            mutate(
              ~variables=
                ChannelCreate.Query.make(
                  ~input={
                    "name": state.values.name |> E.J.fromString,
                    "description":
                      state.values.description |> E.J.O.fromString,
                    "isPublic": state.values.isPublic |> E.Bool.fromString,
                    "isArchived": state.values.isArchived |> E.Bool.fromString,
                    "requireVerification":
                      state.values.requireVerification |> E.Bool.fromString,
                    "knowledgeGraph":
                      state.values.knowledgeGraph |> E.J.O.fromString,
                  },
                  (),
                )##variables,
              ~refetchQueries=[|
                "channels",
                "user",
                "channel",
                "channelMemberships",
              |],
              (),
            )
            |> Js.Promise.then_((result: result('a)) => {
                 switch (result) {
                 | Data(data) => onSuccess'(data)
                 | _ => ()
                 };
                 Js.Promise.resolve();
               })
            |> ignore;

            None;
          },
        ~initialState={
          name: "",
          description: "",
          isPublic: "TRUE",
          isArchived: "FALSE",
          requireVerification: "FALSE",
          knowledgeGraph: "",
        },
        (),
      );

    <FormComponent creating=true reform result />;
  };
};

module Edit = {
  let onSuccess' = data => onSuccess(data##channelUpdate);

  [@react.component]
  let make = (~id, ~channel: Types.channel) => {
    let (mutate, result, _) = ChannelUpdate.Mutation.use();

    let reform =
      Form.use(
        ~validationStrategy=OnDemand,
        ~schema,
        ~onSubmit=
          ({state}) => {
            mutate(
              ~variables=
                ChannelUpdate.Query.make(
                  ~id,
                  ~input={
                    "name": state.values.name |> E.J.fromString,
                    "description":
                      state.values.description |> E.J.O.fromString,
                    "isPublic": state.values.isPublic |> E.Bool.fromString,
                    "isArchived": state.values.isArchived |> E.Bool.fromString,
                    "requireVerification":
                      state.values.requireVerification |> E.Bool.fromString,
                    "knowledgeGraph":
                      state.values.knowledgeGraph |> E.J.O.fromString,
                  },
                  (),
                )##variables,
              ~refetchQueries=[|"channels", "user", "channel"|],
              (),
            )
            |> Js.Promise.then_((result: result('a)) => {
                 switch (result) {
                 | Data(data) => onSuccess'(data)
                 | _ => ()
                 };
                 Js.Promise.resolve();
               })
            |> ignore;

            None;
          },
        ~initialState={
          name: channel.name,
          description: channel.description |> E.O.default(""),
          isPublic: channel.isPublic |> E.Bool.toString,
          isArchived: channel.isArchived |> E.Bool.toString,
          requireVerification: channel.requireVerification |> E.Bool.toString,
          knowledgeGraph: channel.knowledgeGraph |> E.O.default(""),
        },
        (),
      );

    <FormComponent creating=false reform result />;
  };
};