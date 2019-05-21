open Utils;
open MomentRe;
open Foretold__GraphQL;

module FormConfig = {
  type field(_) =
    | Name: field(string)
    | LabelCustom: field(string)
    | LabelSubject: field(string)
    | LabelOnDate: field(string)
    | LabelProperty: field(string)
    | ExpectedResolutionDate: field(string)
    | ResolutionEndpoint: field(string)
    | ShowDescriptionDate: field(string)
    | ShowDescriptionProperty: field(string);

  type state = {
    name: string,
    labelCustom: string,
    labelSubject: string,
    labelOnDate: string,
    labelProperty: string,
    expectedResolutionDate: string,
    resolutionEndpoint: string,
    showDescriptionDate: string,
    showDescriptionProperty: string,
  };

  let get: type value. (state, field(value)) => value =
    (state, field) =>
      switch (field) {
      | Name => state.name
      | LabelCustom => state.labelCustom
      | LabelSubject => state.labelSubject
      | LabelOnDate => state.labelOnDate
      | LabelProperty => state.labelProperty
      | ExpectedResolutionDate => state.expectedResolutionDate
      | ResolutionEndpoint => state.resolutionEndpoint
      | ShowDescriptionDate => state.showDescriptionDate
      | ShowDescriptionProperty => state.showDescriptionProperty
      };

  let set: type value. (state, field(value), value) => state =
    (state, field, value) =>
      switch (field) {
      | Name => {...state, name: value}
      | LabelCustom => {...state, labelCustom: value}
      | LabelSubject => {...state, labelSubject: value}
      | LabelOnDate => {...state, labelOnDate: value}
      | LabelProperty => {...state, labelProperty: value}
      | ExpectedResolutionDate => {...state, expectedResolutionDate: value}
      | ResolutionEndpoint => {...state, resolutionEndpoint: value}
      | ShowDescriptionDate => {...state, showDescriptionDate: value}
      | ShowDescriptionProperty => {...state, showDescriptionProperty: value}
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
    ~initialState={
      name: "",
      labelCustom: "",
      labelSubject: "",
      labelOnDate: "",
      labelProperty: "",
      expectedResolutionDate: "",
      resolutionEndpoint: "",
      showDescriptionDate: "",
      showDescriptionProperty: "",
    },
    ~onSubmit=
      values => {
        let mutate =
          Mutations.BotCreate.mutate(
            mutation,
            values.state.values.name,
            values.state.values.labelCustom,
            values.state.values.labelSubject,
            values.state.values.labelOnDate,
            values.state.values.labelProperty,
            values.state.values.expectedResolutionDate,
            values.state.values.resolutionEndpoint,
            values.state.values.showDescriptionDate,
            values.state.values.showDescriptionProperty,
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
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          send(Form.FieldChangeValue(Name, e))
        )}
      />
    </Antd.Form.Item>
    <Antd.Form.Item>
      {"Description" |> ste |> E.React.inH3}
      <Antd.Input
        value={form.values.description}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          send(Form.FieldChangeValue(Description, e))
        )}
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