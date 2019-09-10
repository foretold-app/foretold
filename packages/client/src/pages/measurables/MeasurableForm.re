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
    channelId: string,
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
    | `channelId
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
    (`channelId, s => s.channelId, (s, channelId) => {...s, channelId}),
  ];
};

module MeasurableReForm = ReForm.Create(Params);

let showForm =
    (
      ~loggedInUser: Types.user,
      ~form: MeasurableReForm.state,
      ~handleSubmit,
      ~handleChange,
      ~creating=true,
      (),
    ) =>
  <AntdForm onSubmit={ReForm.Helpers.handleDomFormSubmit(handleSubmit)}>
    {E.React.showIf(
       creating,
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
           <Antd.Radio value="PERCENTAGE">
             {"Yes/No Event" |> ste}
           </Antd.Radio>
         </Antd.Radio.Group>
       </Form.Item>,
     )}
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
    {loggedInUser.agent
     |> E.O.fmap((agent: Types.agent) =>
          ChannelsGet.component(
            ~channelMemberId=?Some(agent.id),
            ~sortFn=ChannelsGet.sortAsc,
            channels =>
            channels
            |> Array.mapi((index, channel: Types.channel) =>
                 <Antd.Select.Option
                   value={channel.id} key={index |> string_of_int}>
                   {channel.name |> Utils.ste}
                 </Antd.Select.Option>
               )
            |> ReasonReact.array
            |> (
              c =>
                <Form.Item label="Channel">
                  <Antd.Select
                    value={form.values.channelId}
                    onChange={e => handleChange(`channelId, e)}>
                    c
                  </Antd.Select>
                </Form.Item>
            )
          )
        )
     |> E.O.React.defaultNull}
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
         <Form.Item
           help="What are the most extreme values this could possibly take? For example, inches of rain tomorrow has a minimum of 0.">
           <Div flexDirection=`row>
             <Div flex={`num(1.)}>
               <Div flexDirection=`row>
                 <Div flex={`num(1.)}>
                   <div className="ant-form-item-label">
                     <label className="" title="Min">
                       {"Min" |> Utils.ste}
                     </label>
                   </div>
                   <div className="ant-form-item-control">
                     <Antd.Input
                       className=Styles.shortInput
                       value={form.values.min}
                       onChange={e =>
                         handleChange(`min, ReactEvent.Form.target(e)##value)
                       }
                     />
                   </div>
                 </Div>
                 <Div flex={`num(1.)}>
                   <div className="ant-form-item-label">
                     <label className="" title="Max">
                       {"Max" |> Utils.ste}
                     </label>
                   </div>
                   <div className="ant-form-item-control">
                     <Antd.Input
                       className=Styles.shortInput
                       value={form.values.max}
                       onChange={e =>
                         handleChange(`max, ReactEvent.Form.target(e)##value)
                       }
                     />
                   </div>
                 </Div>
               </Div>
             </Div>
             <Div flex={`num(3.)} />
           </Div>
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
    {Primary.User.show(
       loggedInUser,
       <Form.Item
         label="Resolution Endpoint"
         help="If you enter an url that returns a number, this will be called when the resolution date occurs, and entered as a judgement value.">
         <Input
           value={form.values.resolutionEndpoint}
           onChange={ReForm.Helpers.handleDomFormChange(
             handleChange(`resolutionEndpoint),
           )}
         />
       </Form.Item>,
     )}
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
    {Primary.User.show(
       loggedInUser,
       <Form.Item label="Use Entities in Title">
         <Antd.Radio.Group
           value={form.values.showDescriptionProperty}
           defaultValue={form.values.showDescriptionProperty}
           onChange={ReForm.Helpers.handleDomFormChange(
             handleChange(`showDescriptionProperty),
           )}>
           <Antd.Radio value="FALSE"> {"No" |> ste} </Antd.Radio>
           <Antd.Radio value="TRUE">
             {"Yes (Experimental)" |> ste}
           </Antd.Radio>
         </Antd.Radio.Group>
       </Form.Item>,
     )}
    <Form.Item>
      <Button _type=`primary onClick={_ => handleSubmit()}>
        {"Submit" |> ste}
      </Button>
    </Form.Item>
  </AntdForm>;