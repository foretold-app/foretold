open Style.Grid;
open BsReform;

module FormConfig = [%lenses
  type state = {
    id: string,
    name: string,
    body: string,
    channelId: string,
  }
];

module Form = ReForm.Make(FormConfig);

type result('a) = ReasonApolloHooks.Mutation.controledVariantResult('a);

let schema =
  Form.Validation.Schema([|
    Custom(
      Name,
      values =>
        Js.String.length(values.name) < 3 ? Error(Lang.atLeast3) : Valid,
    ),
    Custom(
      Body,
      values =>
        Js.String.length(values.body) < 3 ? Error(Lang.atLeast3) : Valid,
    ),
  |]);

let onSuccess = channelId => {
  Utils.setTimeout(_ => Routing.Url.push(ChannelNotebooks(channelId)), 1000)
  |> ignore;
  ();
};

module FormComponent = {
  [@react.component]
  let make = (~reform: Form.api, ~result: result('a), ~editing: bool) => {
    let onSubmit = event => {
      ReactEvent.Synthetic.preventDefault(event);
      reform.submit();
    };

    <Form.Provider value=reform>
      {switch (result) {
       | Error(_error) => <Sorry />
       | Loading(_)
       | Data(_) => <Spin />
       | _ =>
         let notebookRedux = NotebookRedux.reducer();

         <FC__PageCard.BodyPadding>
           <Form.Field
             field=FormConfig.Name
             render={({handleChange, error, value, validate}) =>
               <Antd.Form.Item label={"Name" |> Utils.ste}>
                 <Antd.Input
                   value
                   onBlur={_ => validate()}
                   onChange={Helpers.handleChange(handleChange)}
                 />
                 <Warning error />
               </Antd.Form.Item>
             }
           />
           <Div flexDirection=`row>
             <Div flex={`num(1.)}>
               <Form.Field
                 field=FormConfig.Body
                 render={({handleChange, error, value, validate}) =>
                   <>
                     <Antd.Input.TextArea
                       style={ReactDOMRe.Style.make(~minHeight="80em", ())}
                       value
                       onBlur={_ => validate()}
                       onChange={Helpers.handleChange(handleChange)}
                     />
                     <Warning error />
                   </>
                 }
               />
               <Antd.Form.Item>
                 <Antd.Button _type=`primary onClick=onSubmit>
                   {"Submit" |> Utils.ste}
                 </Antd.Button>
                 {<div>
                    <NotebookComponents.RemoveNotebookButton
                      notebookId={reform.state.values.id}
                      onCompleted={_ => {
                        LinkType.onClick2(
                          Internal(
                            ChannelNotebooks(reform.state.values.channelId),
                          ),
                        );
                        ();
                      }}
                    />
                  </div>
                  |> E.React2.showIf(editing)}
               </Antd.Form.Item>
             </Div>
             <Div flex={`num(1.)}>
               <NotebookMarkdown
                 blocks={NotebookMarkdown.markdownToBlocks(
                   reform.state.values.body,
                 )}
                 notebookRedux
               />
             </Div>
           </Div>
         </FC__PageCard.BodyPadding>;
       }}
    </Form.Provider>;
  };
};

module Create = {
  [@react.component]
  let make = (~channelId: string) => {
    let (mutate, result, _) = NotebookCreateMutation.Mutation.use();

    let reform =
      Form.use(
        ~validationStrategy=OnDemand,
        ~schema,
        ~onSubmit=
          ({state}) => {
            mutate(
              ~variables=
                NotebookCreateMutation.Query.make(
                  ~input={
                    "channelId": state.values.channelId |> E.J.fromString,
                    "name": state.values.name |> E.J.fromString,
                    "body": state.values.body |> E.J.fromString,
                  },
                  (),
                )##variables,
              ~refetchQueries=[|"getNotebooks"|],
              (),
            )
            |> Js.Promise.then_((result: result('a)) => {
                 switch (result) {
                 | Data(_) => onSuccess(channelId)
                 | _ => ()
                 };
                 Js.Promise.resolve();
               })
            |> ignore;

            None;
          },
        ~initialState={id: "", name: "", body: "", channelId},
        (),
      );

    <FormComponent reform result editing=false />;
  };
};

module Edit = {
  [@react.component]
  let make = (~notebook: Types.notebook) => {
    let (mutate, result, _) = NotebookUpdateMutation.Mutation.use();

    let reform =
      Form.use(
        ~validationStrategy=OnDemand,
        ~schema,
        ~onSubmit=
          ({state}) => {
            mutate(
              ~variables=
                NotebookUpdateMutation.Query.make(
                  ~id=state.values.id,
                  ~input={
                    "name": state.values.name |> E.J.fromString,
                    "body": state.values.body |> E.J.fromString,
                  },
                  (),
                )##variables,
              ~refetchQueries=[|"getNotebook"|],
              (),
            )
            |> Js.Promise.then_((result: result('a)) => {
                 switch (result) {
                 | Data(_) => onSuccess(notebook.channelId)
                 | _ => ()
                 };
                 Js.Promise.resolve();
               })
            |> ignore;

            None;
          },
        ~initialState={
          id: notebook.id,
          name: notebook.name,
          body: notebook.body,
          channelId: notebook.channelId,
        },
        (),
      );

    <FormComponent reform result editing=true />;
  };
};