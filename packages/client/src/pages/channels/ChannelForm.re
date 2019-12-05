[@bs.config {jsx: 3}];

open MomentRe;

module FormConfig = {
  type field(_) =
    | Name: field(string)
    | Description: field(string)
    | IsPublic: field(string)
    | IsArchived: field(string);

  type state = {
    name: string,
    description: string,
    isPublic: string,
    isArchived: string,
  };

  let get: type value. (state, field(value)) => value =
    (state, field) =>
      switch (field) {
      | Name => state.name
      | Description => state.description
      | IsPublic => state.isPublic
      | IsArchived => state.isArchived
      };

  let set: type value. (state, field(value), value) => state =
    (state, field, value) =>
      switch (field) {
      | Name => {...state, name: value}
      | Description => {...state, description: value}
      | IsPublic => {...state, isPublic: value}
      | IsArchived => {...state, isArchived: value}
      };
};

module Form = ReFormNext.Make(FormConfig);

let showForm =
    (~state: Form.state, ~creating=true, ~onSubmit, ~handleChange, ()) => {
  <Antd.Form onSubmit={e => onSubmit()}>
    <Antd.Form.Item label={"Name" |> Utils.ste}>
      <Antd.Input
        value={state.values.name}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          handleChange(FormConfig.Name, e)
        )}
      />
    </Antd.Form.Item>
    <Antd.Form.Item
      label={"Description" |> Utils.ste}
      help={"Markdown supported" |> Utils.ste}>
      <Antd.Input.TextArea
        style={ReactDOMRe.Style.make(~minHeight="30em", ())}
        value={state.values.description}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          handleChange(Description, e)
        )}
      />
    </Antd.Form.Item>
    <Antd.Form.Item label={"Community is public" |> Utils.ste}>
      <AntdSwitch
        checked={state.values.isPublic == "TRUE"}
        onChange={e => handleChange(IsPublic, e ? "TRUE" : "FALSE")}
      />
    </Antd.Form.Item>
    {E.React2.showIf(
       !creating,
       <Antd.Form.Item label={"Archive community" |> Utils.ste}>
         <AntdSwitch
           checked={state.values.isArchived == "TRUE"}
           onChange={e => handleChange(IsArchived, e ? "TRUE" : "FALSE")}
         />
       </Antd.Form.Item>,
     )}
    <Antd.Form.Item>
      <Antd.Button _type=`primary onClick={e => onSubmit()}>
        {"Submit" |> Utils.ste}
      </Antd.Button>
    </Antd.Form.Item>
  </Antd.Form>;
};