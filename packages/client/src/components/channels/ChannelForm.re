open BsReform;

module FormConfig = [%lenses
  type state = {
    name: string,
    description: string,
    isPublic: string,
    isArchived: string,
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
          ? ReSchema.Error("Keep it short!") : Valid,
    ),
    Custom(
      Name,
      values =>
        Js.String.length(values.name) < 3
          ? Error("The name too short.") : Valid,
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
    let onSubmit = event => {
      ReactEvent.Synthetic.preventDefault(event);
      reform.submit();
    };

    <Form.Provider value=reform>
      {switch (result) {
       | Error(_error) => <p> {Lang.networkError |> Utils.ste} </p>
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
                 <Antd.Input.TextArea
                   value
                   style={ReactDOMRe.Style.make(~minHeight="30em", ())}
                   onChange={Helpers.handleChange(handleChange)}
                 />
                 <Warning error />
               </Antd.Form.Item>
             }
           />
           <Form.Field
             field=FormConfig.IsPublic
             render={({handleChange, error, value}) =>
               <Antd.Form.Item label={"Community is public" |> Utils.ste}>
                 <AntdSwitch
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
                    <AntdSwitch
                      checked={value == "TRUE"}
                      onChange={e => handleChange(e ? "TRUE" : "FALSE")}
                    />
                    <Warning error />
                  </Antd.Form.Item>
                }
              />,
            )}
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
                  },
                  (),
                )##variables,
              ~refetchQueries=[|"getChannels", "user", "channel"|],
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
                  },
                  (),
                )##variables,
              ~refetchQueries=[|"getChannels", "user", "channel"|],
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
        },
        (),
      );

    <FormComponent creating=false reform result />;
  };
};