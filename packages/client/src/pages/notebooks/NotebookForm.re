[@bs.config {jsx: 3}];
open Style.Grid;
open BsReform;

module FormConfig = [%lenses
  type state = {
    name: string,
    body: string,
  }
];

module Form = ReForm.Make(FormConfig);

let testName = str => {
  Js.String.length(str) > 3;
};

let testBody = str => {
  Js.String.length(str) > 3;
};

let withForm = (onSubmit, notebook: option(Types.notebook)) => {
  let initialState: FormConfig.state =
    switch (notebook) {
    | Some(notebook) => {name: notebook.name, body: notebook.body}
    | None => {name: "", body: ""}
    };

  Form.use(
    ~initialState,
    ~onSubmit,
    ~schema=
      Form.Validation.Schema([|
        //        Custom(
        //          Name,
        //          values =>
        //            testName(values.name)
        //              ? ReForm.Valid : ReForm.Error(Lang.atLeast3),
        //        ),
        //        Custom(
        //          Body,
        //          values =>
        //            testBody(values.body)
        //              ? ReForm.Valid : ReForm.Error(Lang.atLeast3),
        //        ),
      |]),
    (),
  );
};

module FormFields = {
  [@react.component]
  let make = (~reform: Form.api) => {
    let notebookRedux = NotebookRedux.reducer();

    <FC__PageCard.BodyPadding>
      <Form.Field
        field=FormConfig.Name
        render={({handleChange, error, value}) =>
          <Antd.Form.Item label={"Name" |> Utils.ste}>
            <Antd.Input
              value
              onChange={e => handleChange(ReactEvent.Form.target(e)##value)}
            />
            {error->Belt.Option.getWithDefault("")->React.string}
          </Antd.Form.Item>
        }
      />
      <Div flexDirection=`row>
        <Div flex={`num(1.)}>
          <Form.Field
            field=FormConfig.Body
            render={({handleChange, error, value}) =>
              <>
                <Antd.Input.TextArea
                  style={ReactDOMRe.Style.make(~minHeight="80em", ())}
                  value
                  onChange={e =>
                    handleChange(ReactEvent.Form.target(e)##value)
                  }
                />
                {error->Belt.Option.getWithDefault("")->React.string}
              </>
            }
          />
          <Antd.Form.Item>
            <Antd.Button
              _type=`primary
              onClick={event => {
                ReactEvent.Synthetic.preventDefault(event);
                reform.submit();
              }}>
              {"Submit" |> Utils.ste}
            </Antd.Button>
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
    //    let isEnabled = isFormValid && isFormDirty;
    //
    //      };
    //      | _ => false
    //      | Form.Dirty => true
    //      switch (stateForm) {
    //    let isFormDirty =
    //
    //      };
    //      | _ => true
    //      | (_, ReFormNext.Error(_)) => false
    //      | (ReFormNext.Error(_), _) => false
    //      switch (stateName, stateBody) {
    //    let isFormValid =
    //
    //      };
    //      | _ => <Null />
    //      | ReFormNext.Error(s) => <AntdAlert message=s type_="warning" />
    //      switch (state) {
    //    let error = state =>
    //
    //    let stateForm = state.formState;
    //
    //    let stateBody = getFieldState(Form.Field(Body));
    //    let stateName = getFieldState(Form.Field(Name));
    //    let notebookRedux = NotebookRedux.reducer();
    //
    //    let onSubmit = () => submit();
  };
};

let formFields = reform => <FormFields reform />;