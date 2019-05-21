open Utils;
open MomentRe;
open Foretold__GraphQL;
open Antd;

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

module CMutationForm =
  MutationForm.Make({
    type queryType = Mutations.MeasurableCreate.Query.t;
  });

let formatDate = E.M.format(E.M.format_standard);

let withForm = (mutation, channelId, innerComponentFn) => {
  let initialState: FormConfig.state = {
    name: "",
    labelCustom: "",
    labelSubject: "",
    labelOnDate: MomentRe.momentNow() |> formatDate,
    labelProperty: "",
    expectedResolutionDate: MomentRe.momentNow() |> formatDate,
    resolutionEndpoint: "",
    showDescriptionDate: "FALSE",
    showDescriptionProperty: "FALSE",
  };

  Form.make(
    ~initialState,
    ~onSubmit=
      ({state}) =>
        Mutations.MeasurableCreate.mutate(mutation, state.values, channelId),
    ~schema=Form.Validation.Schema([||]),
    innerComponentFn,
  )
  |> E.React.el;
};

let formFields = (form: Form.state, send, onSubmit) =>
  <Antd.Form onSubmit={e => onSubmit()}>
    <Antd.Form.Item label="Question Type">
      <Antd.Radio.Group
        value={form.values.showDescriptionProperty}
        defaultValue={form.values.showDescriptionProperty}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          send(Form.FieldChangeValue(ShowDescriptionProperty, e))
        )}>
        <Antd.Radio value="FALSE"> {"Simple" |> ste} </Antd.Radio>
        <Antd.Radio value="TRUE">
          {"Subject-Property-Date" |> ste}
        </Antd.Radio>
      </Antd.Radio.Group>
    </Antd.Form.Item>
    {E.React.showIf(
       form.values.showDescriptionProperty == "TRUE",
       <>
         <Antd.Form.Item label="Subject" required=true>
           <Antd.Input
             value={form.values.labelSubject}
             onChange={e => {
               // handleChange(`labelSubject, ReactEvent.Form.target(e)##value);
               Js.log2("labelSubject", e);
               ();
             }}
           />
         </Antd.Form.Item>
         <Antd.Form.Item label="Property" required=true>
           <Antd.Input
             value={form.values.labelProperty}
             onChange={e => {
               // handleChange(`labelProperty, ReactEvent.Form.target(e)##value);
               Js.log2("labelProperty", e);
               ();
             }}
           />
         </Antd.Form.Item>
         <Antd.Form.Item label="Include a Specific Date in Name">
           <AntdSwitch
             checked={form.values.showDescriptionDate == "TRUE"}
             onChange={e => {
               // handleChange(`showDescriptionDate, e ? "TRUE" : "FALSE")
               Js.log2("showDescriptionDate", e);
               ();
             }}
           />
         </Antd.Form.Item>
         {form.values.showDescriptionDate == "TRUE"
            ? <Antd.Form.Item label="'On' Date">
                <DatePicker
                  value={form.values.labelOnDate |> MomentRe.moment}
                  onChange={e => {
                    // handleChange(`expectedResolutionDate, e |> formatDate);
                    // handleChange(`labelOnDate, e |> formatDate);
                    Js.log2("expectedResolutionDate", e);
                    ();
                  }}
                />
              </Antd.Form.Item>
            : <div />}
       </>,
     )}
    {E.React.showIf(
       form.values.showDescriptionProperty == "FALSE",
       <Antd.Form.Item label="Name" required=true>
         <Input
           value={form.values.name}
           onChange={ReForm.Helpers.handleDomFormChange(e =>
             send(Form.FieldChangeValue(Name, e))
           )}
         />
       </Antd.Form.Item>,
     )}
    <Antd.Form.Item label="Description">
      <Input
        value={form.values.labelCustom}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          send(Form.FieldChangeValue(LabelCustom, e))
        )}
      />
    </Antd.Form.Item>
    <Antd.Form.Item
      label="Resolution Endpoint"
      help="If you enter an url that returns a number, this will be called when the resolution date occurs, and entered as a judgement value.">
      <Input
        value={form.values.resolutionEndpoint}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          send(Form.FieldChangeValue(ResolutionEndpoint, e))
        )}
      />
    </Antd.Form.Item>
    <Antd.Form.Item
      label="Expected Resolution Date"
      help="When do you expect this will be resolvable by? You will get a notification when this date occurs.">
      <DatePicker
        value={
          form.values.expectedResolutionDate |> MomentRe.momentDefaultFormat
        }
        onChange={e => {
          send(
            Form.FieldChangeValue(ExpectedResolutionDate, e |> formatDate),
          );
          ();
        }}
        disabled={form.values.showDescriptionDate == "TRUE"}
      />
    </Antd.Form.Item>
    <Antd.Form.Item>
      <Button _type=`primary onClick={_ => onSubmit()}>
        {"Submit" |> ste}
      </Button>
    </Antd.Form.Item>
  </Antd.Form>;

let component = ReasonReact.statelessComponent("MeasurementForm");

let make = (~channelId, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ => {
    let head = SLayout.Header.textDiv("New Question");

    let body =
      Mutations.MeasurableCreate.withMutation((mutation, data) =>
        withForm(mutation, channelId, ({send, state}) =>
          CMutationForm.showWithLoading(
            ~result=data.result,
            ~form=formFields(state, send, () => send(Form.Submit)),
            (),
          )
        )
      )
      |> E.React.el;

    SLayout.LayoutConfig.make(~head, ~body) |> layout;
  },
};