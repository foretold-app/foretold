open Utils;
open Antd;

open Style.Grid;

let formatDate = E.M.format(E.M.format_standard);

module Styles = {
  open Css;
  let shortInput = [width(`em(6.))] |> style;
};

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
    min: string,
    max: string,
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
    | `min
    | `max
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
    (`min, s => s.min, (s, min) => {...s, min}),
    (`max, s => s.max, (s, max) => {...s, max}),
  ];
};

module MeasurableReForm = ReForm.Create(Params);

let dataSource =
  EKen.Things.getAll
  |> EKen.Things.withNames
  |> E.A.fmap((r: Graph_T.T.thing) =>
       {"key": r |> Graph_T.Thing.id, "id": r |> Graph_T.Thing.id}
     );

let showForm = (~form: MeasurableReForm.state, ~handleSubmit, ~handleChange) =>
  <AntdForm onSubmit={ReForm.Helpers.handleDomFormSubmit(handleSubmit)}>
    <Form.Item
      label="Question Type"
      help="Number example: 'How many inches of rain will there be tomorrow?' Yes/No example: 'Will it rain tomorrow?'">
      <Antd.Radio.Group
        value={form.values.valueType}
        defaultValue={form.values.valueType}
        onChange={ReForm.Helpers.handleDomFormChange(
          handleChange(`valueType),
        )}>
        <Antd.Radio value="FLOAT"> {"Number" |> ste} </Antd.Radio>
        <Antd.Radio value="PERCENTAGE"> {"Yes/No Event" |> ste} </Antd.Radio>
      </Antd.Radio.Group>
    </Form.Item>
    {E.React.showIf(
       form.values.showDescriptionProperty == "FALSE",
       <>
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
    {E.React.showIf(
       form.values.showDescriptionProperty == "TRUE",
       <>
         <p />
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
       form.values.valueType == "FLOAT",
       <>
         <Div flexDirection=`row>
           <Div flex={`num(1.)}>
             <Div flexDirection=`row>
               <Div flex={`num(1.)}>
                 <Form.Item label="Min">
                   <Antd.Input
                     className=Styles.shortInput
                     htmlType="number"
                     value={form.values.min}
                     onChange={e =>
                       handleChange(`min, ReactEvent.Form.target(e)##value)
                     }
                   />
                 </Form.Item>
               </Div>
               <Div flex={`num(1.)}>
                 <Form.Item label="Max">
                   <Antd.Input
                     className=Styles.shortInput
                     htmlType="number"
                     value={form.values.max}
                     onChange={e =>
                       handleChange(`max, ReactEvent.Form.target(e)##value)
                     }
                   />
                 </Form.Item>
               </Div>
             </Div>
           </Div>
           <Div flex={`num(3.)} />
         </Div>
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
    <Form.Item label="Use Entities in Title">
      <Antd.Radio.Group
        value={form.values.showDescriptionProperty}
        defaultValue={form.values.showDescriptionProperty}
        onChange={ReForm.Helpers.handleDomFormChange(
          handleChange(`showDescriptionProperty),
        )}>
        <Antd.Radio value="FALSE"> {"No" |> ste} </Antd.Radio>
        <Antd.Radio value="TRUE"> {"Yes (Experimental)" |> ste} </Antd.Radio>
      </Antd.Radio.Group>
    </Form.Item>
    <Form.Item>
      <Button _type=`primary onClick={_ => handleSubmit()}>
        {"Submit" |> ste}
      </Button>
    </Form.Item>
  </AntdForm>;