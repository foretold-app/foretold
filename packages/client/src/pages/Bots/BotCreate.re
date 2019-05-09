open Utils;
open MomentRe;
open Foretold__GraphQL;

type competitorType = [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE];

module FormConfig = {
  type field(_) =
    | Name: field(string)
    | Description: field(string)
    | CompetitorType: field(competitorType);
  type state = {
    name: string,
    description: string,
    competitorType,
  };

  let get: type value. (state, field(value)) => value =
    (state, field) =>
      switch (field) {
      | Name => state.name
      | Description => state.description
      | CompetitorType => state.competitorType
      };

  let set: type value. (state, field(value), value) => state =
    (state, field, value) =>
      switch (field) {
      | Name => {...state, name: value}
      | Description => {...state, description: value}
      | CompetitorType => {...state, competitorType: value}
      };
};

module Form = ReFormNext.Make(FormConfig);

let component = ReasonReact.statelessComponent("BotForm");

module CMutationForm =
  MutationForm.Make({
    type queryType = Mutations.BotCreate.Query.t;
  });

let withForm = (mutation, innerComponentFn) =>
  Form.make(
    ~initialState={name: "", description: "", competitorType: `COMPETITIVE},
    ~onSubmit=
      values => {
        let mutate =
          Mutations.BotCreate.mutate(
            mutation,
            values.state.values.name,
            values.state.values.description,
            values.state.values.competitorType,
          );
        ();
      },
    ~schema=Form.Validation.Schema([||]),
    innerComponentFn,
  )
  |> E.React.el;

let formFields = (form: Form.state, send, onSubmit) =>
  <Antd.Form onSubmit={e => onSubmit()}>
    <Antd.Form.Item>
      {"Name" |> ste |> E.React.inH3}
      <Antd.Input
        value={form.values.name}
        onChange={
          ReForm.Helpers.handleDomFormChange(e =>
            send(Form.FieldChangeValue(Name, e))
          )
        }
      />
    </Antd.Form.Item>
    <Antd.Form.Item>
      {"Description" |> ste |> E.React.inH3}
      <Antd.Input
        value={form.values.description}
        onChange={
          ReForm.Helpers.handleDomFormChange(e =>
            send(Form.FieldChangeValue(Description, e))
          )
        }
      />
    </Antd.Form.Item>
    <Antd.Form.Item>
      <Antd.Button _type=`primary onClick={_ => onSubmit()}>
        {"Submit" |> ste}
      </Antd.Button>
    </Antd.Form.Item>
  </Antd.Form>;

let make = (~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ =>
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Make a New Bot"),
      ~body=
        Mutations.BotCreate.withMutation((mutation, data) =>
          withForm(mutation, ({send, state}) =>
            CMutationForm.showWithLoading(
              ~result=data.result,
              ~form=formFields(state, send, () => send(Form.Submit)),
              (),
            )
          )
        ),
    )
    |> layout,
};