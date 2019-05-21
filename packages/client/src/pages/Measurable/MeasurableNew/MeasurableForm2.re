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

  // @todo: unduplicate
  type state = Mutations.MeasurableCreate.values;

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

let component = ReasonReact.statelessComponent("MeasurementForm");

module CMutationForm =
  MutationForm.Make({
    type queryType = Mutations.MeasurableCreate.Query.t;
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
          Mutations.MeasurableCreate.mutate(
            mutation,
            values.state.values,
            "sdfsdf" // @todo:
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
      <Antd.Button _type=`primary onClick={_ => onSubmit()}>
        {"Submit" |> ste}
      </Antd.Button>
    </Antd.Form.Item>
  </Antd.Form>;

let make = (~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ => {
    let head = SLayout.Header.textDiv("New Question");

    let body =
      Mutations.MeasurableCreate.withMutation((mutation, data) =>
        withForm(mutation, ({send, state}) =>
          CMutationForm.showWithLoading(
            ~result=data.result,
            ~form=formFields(state, send, () => send(Form.Submit)),
            (),
          )
        )
      );

    SLayout.LayoutConfig.make(~head, ~body) |> layout;
  },
};