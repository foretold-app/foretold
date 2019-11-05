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
          values => values.name == "" ? Error("Can't be empty") : Valid,
        ),
      |]),
    innerComponentFn,
  )
  |> E.React2.el;
};

let formFields = (state: Form.state, send, onSubmit) =>
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
      </Antd.Form.Item>
      <Antd.Form.Item>
        <Antd.Button _type=`primary onClick={_ => onSubmit()}>
          {"Submit" |> ReasonReact.string}
        </Antd.Button>
      </Antd.Form.Item>
    </Antd.Form>
  </FC__PageCard.BodyPadding>;
