[@bs.config {jsx: 3}];

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
  let exp = () => [%re "/^[a-z0-9._]{3,255}$/i"];
  let res = exp() |> Js.Re.exec(str);
  switch (res) {
  | Some(_) => true
  | _ => false
  };
};

let testBody = str => {
  let exp = () => [%re "/^[a-z0-9._]{3,16384}$/i"];
  let res = exp() |> Js.Re.exec(str);
  switch (res) {
  | Some(_) => true
  | _ => false
  };
};

let withForm = (onSubmit, notebook: option(Types.notebook), innerComponentFn) => {
  let initialState: FormConfig.state =
    switch (notebook) {
    | Some(notebook) => {name: notebook.name, body: notebook.body}
    | None => {name: "", body: ""}
    };

  Form.make(
    ~initialState,
    ~onSubmit,
    ~schema=
      Form.Validation.Schema([|
        Custom(
          Name,
          values =>
            testName(values.name)
              ? Valid : Error("At least 3 charachters are needed."),
        ),
        Custom(
          Body,
          values =>
            testBody(values.body)
              ? Valid : Error("At least 3 charachters are needed."),
        ),
      |]),
    innerComponentFn,
  )
  |> E.React2.el;
};

let formFields = (state: Form.state, send, getFieldState) => {
  let onSubmit = () => send(Form.Submit);

  let stateName = getFieldState(Form.Field(Name));
  let stateBody = getFieldState(Form.Field(Body));

  let stateForm = state.formState;

  let error = state =>
    switch (state) {
    | Form.Error(s) => <AntdAlert message=s type_="warning" />
    | _ => <Null />
    };

  let isFormValid =
    switch (stateName, stateBody) {
    | (Form.Error(_), _) => false
    | (_, Form.Error(_)) => false
    | _ => true
    };

  let isFormDirty =
    switch (stateForm) {
    | Form.Dirty => true
    | _ => false
    };

  let isEnabled = isFormValid && isFormDirty;

  <FC__PageCard.BodyPadding>
    <Antd.Form onSubmit={e => onSubmit()}>
      <Antd.Form.Item>
        {"Name" |> ReasonReact.string |> E.React2.inH3}
        <Antd.Input
          value={state.values.name}
          onChange={ReForm.Helpers.handleDomFormChange(e =>
            send(Form.FieldChangeValue(Name, e))
          )}
        />
        {error(stateName)}
      </Antd.Form.Item>
      <Antd.Form.Item label={"Body" |> Utils.ste} help={"" |> Utils.ste}>
        <Antd.Input.TextArea
          style={ReactDOMRe.Style.make(~minHeight="12em", ())}
          value={state.values.body}
          onChange={e =>
            send(
              Form.FieldChangeValue(Body, ReactEvent.Form.target(e)##value),
            )
          }
        />
        {error(stateBody)}
      </Antd.Form.Item>
      <Antd.Form.Item>
        <Antd.Button
          _type=`primary onClick={_ => onSubmit()} disabled={!isEnabled}>
          {"Submit" |> ReasonReact.string}
        </Antd.Button>
      </Antd.Form.Item>
    </Antd.Form>
  </FC__PageCard.BodyPadding>;
};