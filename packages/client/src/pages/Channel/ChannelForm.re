open Utils;
open MomentRe;
open Antd;

module Params = {
  type state = {
    name: string,
    description: string,
    isPublic: string,
    isArchived: string,
  };
  type fields = [ | `name | `description | `isPublic | `isArchived];
  let lens = [
    (`name, s => s.name, (s, name) => {...s, name}),
    (
      `description,
      s => s.description,
      (s, description) => {...s, description},
    ),
    (`isPublic, s => s.isPublic, (s, isPublic) => {...s, isPublic}),
    (`isArchived, s => s.isArchived, (s, isArchived) => {...s, isArchived}),
  ];
};

module FormUI = ReForm.Create(Params);

let showForm =
    (~form: FormUI.state, ~handleSubmit, ~handleChange, ~creating=true, ()) =>
  <Form onSubmit={ReForm.Helpers.handleDomFormSubmit(handleSubmit)}>
    <Form.Item>
      {"Name" |> ste |> E.React.inH3}
      <Input
        value={form.values.name}
        onChange={ReForm.Helpers.handleDomFormChange(handleChange(`name))}
      />
    </Form.Item>
    <Form.Item>
      {"Description" |> ste |> E.React.inH3}
      <Input
        value={form.values.description}
        onChange={ReForm.Helpers.handleDomFormChange(
          handleChange(`description),
        )}
      />
    </Form.Item>
    <Form.Item>
      {"Make Public?" |> ste |> E.React.inH3}
      <AntdSwitch
        checked={form.values.isPublic == "TRUE"}
        onChange={e => handleChange(`isPublic, e ? "TRUE" : "FALSE")}
      />
    </Form.Item>
    {E.React.showIf(
       !creating,
       <Form.Item>
         {"Make Archived?" |> ste |> E.React.inH3}
         <AntdSwitch
           checked={form.values.isArchived == "TRUE"}
           onChange={e => handleChange(`isArchived, e ? "TRUE" : "FALSE")}
         />
       </Form.Item>,
     )}
    <Form.Item>
      <Button _type=`primary onClick={_ => handleSubmit()}>
        {"Submit" |> ste}
      </Button>
    </Form.Item>
  </Form>;