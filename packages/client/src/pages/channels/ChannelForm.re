[@bs.config {jsx: 3}];

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

module FormComponent = {
  [@react.component]
  let make =
      (
        ~creating,
        ~onSuccess,
        ~reform: Form.api,
        ~result: ReasonApolloHooks.Mutation.controledVariantResult('a),
      ) => {
    let onSubmit = event => {
      ReactEvent.Synthetic.preventDefault(event);
      reform.submit();
    };

    <Form.Provider value=reform>
      {switch (result) {
       | Error(_error) => <p> {"Something went wrong..." |> Utils.ste} </p>
       | Data(data) => onSuccess(data)
       | _ =>
         <Antd.Form onSubmit>
           <Form.Field
             field=FormConfig.Name
             render={({handleChange, error, value}) =>
               <Antd.Form.Item label={"Name" |> Utils.ste}>
                 <Antd.Input
                   value
                   onChange={event =>
                     ReactEvent.Form.target(event)##value |> handleChange
                   }
                 />
                 <p> {error->Belt.Option.getWithDefault("") |> Utils.ste} </p>
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
                   style={ReactDOMRe.Style.make(~minHeight="30em", ())}
                   value
                   onChange={event =>
                     ReactEvent.Form.target(event)##value |> handleChange
                   }
                 />
                 <p> {error->Belt.Option.getWithDefault("") |> Utils.ste} </p>
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
                 <p> {error->Belt.Option.getWithDefault("") |> Utils.ste} </p>
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
                    <p>
                      {error->Belt.Option.getWithDefault("") |> Utils.ste}
                    </p>
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
  let onSuccess = data => {
    switch (data##channelCreate) {
    | Some(channel) => Routing.Url.push(ChannelShow(channel##id))
    | _ => ()
    };
    <Null />;
  };

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
              (),
            )
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

    <FormComponent creating=true reform result onSuccess />;
  };
};

module Edit = {
  let onSuccess = data => {
    switch (data##channelUpdate) {
    | Some(channel) => Routing.Url.push(ChannelShow(channel##id))
    | _ => ()
    };
    <Null />;
  };

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
              (),
            )
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

    <FormComponent creating=false reform result onSuccess />;
  };
};