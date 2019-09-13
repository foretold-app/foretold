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

let showForm = (~state: Form.state, ~creating=true, ~onSubmit, ~send, ()) =>
  <Antd.Form onSubmit={e => onSubmit()}>
    <Antd.Form.Item>
      {"Name" |> Utils.ste |> E.React.inH3}
      <Antd.Input
        value={state.values.name}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          send(Form.FieldChangeValue(Name, e))
        )}
      />
    </Antd.Form.Item>
    <Antd.Form.Item>
      {"Description" |> Utils.ste |> E.React.inH3}
      <Antd.Input
        value={state.values.description}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          send(Form.FieldChangeValue(Description, e))
        )}
      />
    </Antd.Form.Item>
    <Antd.Form.Item>
      {"Make Community Public?" |> Utils.ste |> E.React.inH3}
      <AntdSwitch
        checked={state.values.isPublic == "TRUE"}
        onChange={e =>
          send(Form.FieldChangeValue(IsPublic, e ? "TRUE" : "FALSE"))
        }
      />
    </Antd.Form.Item>
    {E.React.showIf(
       !creating,
       <Antd.Form.Item>
         {"Archive Community?" |> Utils.ste |> E.React.inH3}
         <AntdSwitch
           checked={state.values.isArchived == "TRUE"}
           onChange={e =>
             send(Form.FieldChangeValue(IsArchived, e ? "TRUE" : "FALSE"))
           }
         />
       </Antd.Form.Item>,
     )}
    <Antd.Form.Item>
      <Antd.Button _type=`primary onClick={e => onSubmit()}>
        {"Submit" |> Utils.ste}
      </Antd.Button>
    </Antd.Form.Item>
  </Antd.Form>;