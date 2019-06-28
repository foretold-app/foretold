open Utils;
open MomentRe;
open Antd;

let formatDate = E.M.format(E.M.format_standard);

module Params = {
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
    valueType: string,
  };

  type fields = [
    | `name
    | `labelCustom
    | `labelSubject
    | `labelProperty
    | `labelOnDate
    | `expectedResolutionDate
    | `resolutionEndpoint
    | `showDescriptionDate
    | `showDescriptionProperty
    | `valueType
  ];

  let lens = [
    (`name, s => s.name, (s, name) => {...s, name}),
    (
      `labelCustom,
      s => s.labelCustom,
      (s, labelCustom) => {...s, labelCustom},
    ),
    (
      `labelSubject,
      s => s.labelSubject,
      (s, labelSubject) => {...s, labelSubject},
    ),
    (
      `labelProperty,
      s => s.labelProperty,
      (s, labelProperty) => {...s, labelProperty},
    ),
    (
      `showDescriptionDate,
      s => s.showDescriptionDate,
      (s, showDescriptionDate) => {...s, showDescriptionDate},
    ),
    (
      `showDescriptionProperty,
      s => s.showDescriptionProperty,
      (s, showDescriptionProperty) => {...s, showDescriptionProperty},
    ),
    (
      `labelOnDate,
      s => s.labelOnDate,
      (s, labelOnDate) => {...s, labelOnDate},
    ),
    (
      `expectedResolutionDate,
      s => s.expectedResolutionDate,
      (s, expectedResolutionDate) => {...s, expectedResolutionDate},
    ),
    (
      `resolutionEndpoint,
      s => s.resolutionEndpoint,
      (s, resolutionEndpoint) => {...s, resolutionEndpoint},
    ),
    (`valueType, s => s.valueType, (s, valueType) => {...s, valueType}),
  ];
};

module SignUpForm = ReForm.Create(Params);

let dataSource =
  EKen.Things.getAll
  |> EKen.Things.withNames
  |> E.A.fmap((r: Graph_T.T.thing) =>
       {"key": r |> Graph_T.Thing.id, "id": r |> Graph_T.Thing.id}
     );

let showForm = (~form: SignUpForm.state, ~handleSubmit, ~handleChange) =>
  <AntdForm onSubmit={ReForm.Helpers.handleDomFormSubmit(handleSubmit)}>
    <Form.Item label="Question Type">
      <Antd.Radio.Group
        value={form.values.showDescriptionProperty}
        defaultValue={form.values.showDescriptionProperty}
        onChange={ReForm.Helpers.handleDomFormChange(
          handleChange(`showDescriptionProperty),
        )}>
        <Antd.Radio value="FALSE"> {"Simple" |> ste} </Antd.Radio>
        <Antd.Radio value="TRUE">
          {"Subject-Property-Date" |> ste}
        </Antd.Radio>
      </Antd.Radio.Group>
    </Form.Item>
    {E.React.showIf(
       form.values.showDescriptionProperty == "TRUE",
       <>
         <p>
           {"Note: you must enter entity IDs in the Subject & Value fields. "
            ++ "Example: @orgs/companies/tesla"
            |> ste}
         </p>
         <p>
           {"It is currently not possible to add or modify entities. "
            ++ "Contact Ozzie for information regarding these."
            |> ste}
         </p>
         <Form.Item label="Subject" required=true>
           <Antd.Input
             value={form.values.labelSubject}
             onChange={e =>
               handleChange(`labelSubject, ReactEvent.Form.target(e)##value)
             }
           />
         </Form.Item>
         <Form.Item label="Property" required=true>
           <Antd.Input
             value={form.values.labelProperty}
             onChange={e =>
               handleChange(`labelProperty, ReactEvent.Form.target(e)##value)
             }
           />
         </Form.Item>
         <Form.Item label="Include a Specific Date in Name">
           <AntdSwitch
             checked={form.values.showDescriptionDate == "TRUE"}
             onChange={e =>
               handleChange(`showDescriptionDate, e ? "TRUE" : "FALSE")
             }
           />
         </Form.Item>
         {form.values.showDescriptionDate == "TRUE"
            ? <Form.Item label="'On' Date">
                <DatePicker
                  value={form.values.labelOnDate |> MomentRe.moment}
                  onChange={e => {
                    handleChange(`labelOnDate, e |> formatDate);
                    handleChange(`expectedResolutionDate, e |> formatDate);
                  }}
                />
              </Form.Item>
            : <div />}
       </>,
     )}
    {E.React.showIf(
       form.values.showDescriptionProperty == "FALSE",
       <>
         <Form.Item label="Result Type">
           <Antd.Radio.Group
             value={form.values.valueType}
             defaultValue={form.values.valueType}
             onChange={ReForm.Helpers.handleDomFormChange(
               handleChange(`valueType),
             )}>
             <Antd.Radio value="FLOAT"> {"Distribution" |> ste} </Antd.Radio>
             <Antd.Radio value="PERCENTAGE"> {"Binary" |> ste} </Antd.Radio>
           </Antd.Radio.Group>
         </Form.Item>
         <Form.Item label="Name" required=true>
           <Input
             value={form.values.name}
             onChange={ReForm.Helpers.handleDomFormChange(
               handleChange(`name),
             )}
           />
         </Form.Item>
       </>,
     )}
    <Form.Item label="Description" help="Markdown supported.">
      <Input.TextArea
        style={ReactDOMRe.Style.make(~minHeight="16em", ())}
        value={form.values.labelCustom}
        onChange={ReForm.Helpers.handleDomFormChange(
          handleChange(`labelCustom),
        )}
      />
    </Form.Item>
    <Form.Item
      label="Resolution Endpoint"
      help="If you enter an url that returns a number, this will be called when the resolution date occurs, and entered as a judgement value.">
      <Input
        value={form.values.resolutionEndpoint}
        onChange={ReForm.Helpers.handleDomFormChange(
          handleChange(`resolutionEndpoint),
        )}
      />
    </Form.Item>
    <Form.Item
      label="Expected Resolution Date"
      help="When do you expect this will be resolvable by? You will get a notification when this date occurs.">
      <DatePicker
        value={
          form.values.expectedResolutionDate |> MomentRe.momentDefaultFormat
        }
        onChange={e => handleChange(`expectedResolutionDate, e |> formatDate)}
        disabled={form.values.showDescriptionDate == "TRUE"}
      />
    </Form.Item>
    <Form.Item>
      <Button _type=`primary onClick={_ => handleSubmit()}>
        {"Submit" |> ste}
      </Button>
    </Form.Item>
  </AntdForm>;