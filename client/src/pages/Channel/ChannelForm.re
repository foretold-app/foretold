open Utils;
open Rationale;
open MomentRe;
open Antd;

let formatDate = Moment.format("MMM DD, YYYY HH:MM:SS");
module SignUpParams = {
  type state = {
    name: string,
    description: string,
    isPublic: string,
  };
  type fields = [ | `name | `description | `isPublic];
  let lens = [
    (`name, s => s.name, (s, name) => {...s, name}),
    (
      `description,
      s => s.description,
      (s, description) => {...s, description},
    ),
    (`isPublic, s => s.isPublic, (s, isPublic) => {...s, isPublic}),
  ];
};

module SignUpForm = ReForm.Create(SignUpParams);

let showForm = (~form: SignUpForm.state, ~handleSubmit, ~handleChange) =>
  <form onSubmit={ReForm.Helpers.handleDomFormSubmit(handleSubmit)}>
    <Form>
      <Form.Item>
        <h3> {"Channel Name" |> ste} </h3>
        <Input
          value={form.values.name}
          onChange={ReForm.Helpers.handleDomFormChange(handleChange(`name))}
        />
      </Form.Item>
      <Form.Item>
        <h3> {"Description" |> ste} </h3>
        <Input
          value={form.values.description}
          onChange={
            ReForm.Helpers.handleDomFormChange(handleChange(`description))
          }
        />
      </Form.Item>
      <Form.Item>
        <h3> {"Make Public?" |> ste} </h3>
        <AntdSwitch
          checked={form.values.isPublic == "TRUE"}
          onChange={e => handleChange(`isPublic, e ? "TRUE" : "FALSE")}
        />
      </Form.Item>
      <Form.Item>
        <Button _type=`primary onClick={_ => handleSubmit()}>
          {"Submit" |> ste}
        </Button>
      </Form.Item>
    </Form>
  </form>;