[@bs.config {jsx: 3}];
open Style.Grid;

module FormConfig = {
  type field(_) =
    | Name: field(string)
    | Body: field(string);

  type state = {
    name: string,
    body: string,
  };

  let get: type value. (state, field(value)) => value =
    (state, field) =>
      switch (field) {
      | Name => state.name
      | Body => state.body
      };

  let set: type value. (state, field(value), value) => state =
    (state, field, value) =>
      switch (field) {
      | Name => {...state, name: value}
      | Body => {...state, body: value}
      };
};

module Form = ReFormNext.Make(FormConfig);

let testName = str => {
  Js.String.length(str) > 3;
};

let testBody = str => {
  Js.String.length(str) > 3;
};

let withForm = (onSubmit, notebook: option(Types.notebook), innerComponentFn) => {
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
        Custom(
          Name,
          values => testName(values.name) ? Valid : Error(Lang.atLeast3),
        ),
        Custom(
          Body,
          values => testBody(values.body) ? Valid : Error(Lang.atLeast3),
        ),
      |]),
    (),
  )
  |> innerComponentFn;
};

module FormFields = {
  [@react.component]
  let make = (~state: Form.state, ~handleChange, ~getFieldState, ~submit) => {
    let onSubmit = () => submit();

    let notebookRedux = NotebookRedux.reducer();
    let stateName = getFieldState(Form.Field(Name));
    let stateBody = getFieldState(Form.Field(Body));

    let stateForm = state.formState;

    let error = state =>
      switch (state) {
      | ReFormNext.Error(s) => <AntdAlert message=s type_="warning" />
      | _ => <Null />
      };

    let isFormValid =
      switch (stateName, stateBody) {
      | (ReFormNext.Error(_), _) => false
      | (_, ReFormNext.Error(_)) => false
      | _ => true
      };

    let isFormDirty =
      switch (stateForm) {
      | Form.Dirty => true
      | _ => false
      };

    let isEnabled = isFormValid && isFormDirty;

    <FC__PageCard.BodyPadding>
      <Antd.Form.Item label={"Name" |> Utils.ste}>
        <Antd.Input
          value={state.values.name}
          onChange={ReForm.Helpers.handleDomFormChange(e =>
            handleChange(FormConfig.Name, e)
          )}
        />
        {error(stateName)}
      </Antd.Form.Item>
      <Div flexDirection=`row>
        <Div flex={`num(1.)}>
          <Antd.Input.TextArea
            style={ReactDOMRe.Style.make(~minHeight="80em", ())}
            value={state.values.body}
            onChange={e =>
              handleChange(Body, ReactEvent.Form.target(e)##value)
            }
          />
          {error(stateBody)}
          <Antd.Form.Item>
            <Antd.Button
              _type=`primary onClick={_ => onSubmit()} disabled={!isEnabled}>
              {"Submit" |> Utils.ste}
            </Antd.Button>
          </Antd.Form.Item>
        </Div>
        <Div flex={`num(1.)}>
          <NotebookMarkdown
            blocks={NotebookMarkdown.markdownToBlocks(state.values.body)}
            notebookRedux
          />
        </Div>
      </Div>
    </FC__PageCard.BodyPadding>;
  };
};

let formFields = (state: Form.state, handleChange, getFieldState, submit) =>
  <FormFields state handleChange getFieldState submit />;