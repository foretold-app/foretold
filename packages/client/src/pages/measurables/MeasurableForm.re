[@bs.config {jsx: 3}];

open Utils;
open Antd;

open Style.Grid;

let formatDate = E.M.format(E.M.format_standard);

module Styles = {
  open Css;
  let shortInput = [width(`em(6.))] |> style;
};

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
    | ShowDescriptionProperty: field(string)
    | ValueType: field(string)
    | Min: field(string)
    | Max: field(string)
    | ChannelId: field(string);

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
      | ValueType => state.valueType
      | Min => state.min
      | Max => state.max
      | ChannelId => state.channelId
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
      | ValueType => {...state, valueType: value}
      | Min => {...state, min: value}
      | Max => {...state, max: value}
      | ChannelId => {...state, channelId: value}
      };
};

module Form = ReFormNext.Make(FormConfig);

let showForm =
    (~state: Form.state, ~handleChange, ~creating=true, ~onSubmit, ()) =>
  <Providers.AppContext.Consumer>
    ...{({loggedUser}) =>
      switch (loggedUser) {
      | Some(loggedUser) =>
        <AntdForm onSubmit={_e => onSubmit()}>
          {E.React2.showIf(
             creating,
             <Antd.Form.Item
               label={"Question Type" |> Utils.ste}
               required=true
               help={
                 "Number example: 'How many inches of rain will there be tomorrow?' Binary example: 'Will it rain tomorrow?'"
                 |> Utils.ste
               }>
               <Antd.Radio.Group
                 value={state.values.valueType}
                 defaultValue={state.values.valueType}
                 onChange={ReForm.Helpers.handleDomFormChange(e =>
                   handleChange(FormConfig.ValueType, e)
                 )}>
                 <Antd.Radio value="FLOAT"> {"Number" |> ste} </Antd.Radio>
                 <Antd.Radio value="PERCENTAGE">
                   {"Binary" |> ste}
                 </Antd.Radio>
               </Antd.Radio.Group>
             </Antd.Form.Item>,
           )}
          {E.React2.showIf(
             state.values.showDescriptionProperty == "FALSE",
             <>
               <Antd.Form.Item
                 label={"Question Title" |> Utils.ste} required=true>
                 <Input
                   value={state.values.name}
                   onChange={ReForm.Helpers.handleDomFormChange(e =>
                     handleChange(Name, e)
                   )}
                 />
               </Antd.Form.Item>
             </>,
           )}
          {E.React2.showIf(
             !creating,
             loggedUser.agent
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
                        <Antd.Form.Item label={"Community" |> Utils.ste}>
                          <Antd.Select
                            value={state.values.channelId}
                            onChange={e => handleChange(ChannelId, e)}>
                            c
                          </Antd.Select>
                        </Antd.Form.Item>
                    )
                  )
                )
             |> E.O.React.defaultNull,
           )}
          {E.React2.showIf(
             state.values.showDescriptionProperty == "TRUE",
             <>
               <p />
               <p>
                 {"Note: you must enter entity IDs in the Subject & Value fields. "
                  ++ "Example: @orgs/companies/tesla"
                  |> ste}
               </p>
               <p>
                 {"Adding and modifying entities is currently not supported."
                  |> ste}
               </p>
               <Antd.Form.Item label={"Subject" |> Utils.ste} required=true>
                 <Antd.Input
                   value={state.values.labelSubject}
                   onChange={e =>
                     handleChange(
                       LabelSubject,
                       ReactEvent.Form.target(e)##value,
                     )
                   }
                 />
               </Antd.Form.Item>
               <Antd.Form.Item label={"Property" |> Utils.ste} required=true>
                 <Antd.Input
                   value={state.values.labelProperty}
                   onChange={e =>
                     handleChange(
                       LabelProperty,
                       ReactEvent.Form.target(e)##value,
                     )
                   }
                 />
               </Antd.Form.Item>
               <Antd.Form.Item
                 label={"Include a Specific Date in Name" |> Utils.ste}>
                 <AntdSwitch
                   checked={state.values.showDescriptionDate == "TRUE"}
                   onChange={e =>
                     handleChange(ShowDescriptionDate, e ? "TRUE" : "FALSE")
                   }
                 />
               </Antd.Form.Item>
               {state.values.showDescriptionDate == "TRUE"
                  ? <Antd.Form.Item label={"'On' Date" |> Utils.ste}>
                      <DatePicker
                        value={state.values.labelOnDate |> MomentRe.moment}
                        onChange={e => {
                          handleChange(LabelOnDate, e |> formatDate);
                          handleChange(
                            ExpectedResolutionDate,
                            e |> formatDate,
                          );
                          _ => ();
                        }}
                      />
                    </Antd.Form.Item>
                  : <div />}
             </>,
           )}
          <Antd.Form.Item
            label={"Description" |> Utils.ste}
            help={"Markdown supported." |> Utils.ste}>
            <Input.TextArea
              style={ReactDOMRe.Style.make(~minHeight="12em", ())}
              value={state.values.labelCustom}
              onChange={e =>
                handleChange(LabelCustom, ReactEvent.Form.target(e)##value)
              }
            />
          </Antd.Form.Item>
          {E.React2.showIf(
             state.values.valueType == "FLOAT",
             <>
               <Antd.Form.Item
                 help={
                   "What are the most extreme values this could possibly take? For example, inches of rain tomorrow has a minimum of 0. These are optional, and in many cases not relevant."
                   |> Utils.ste
                 }>
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
                             value={state.values.min}
                             onChange={e =>
                               handleChange(
                                 Min,
                                 ReactEvent.Form.target(e)##value,
                               )
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
                             value={state.values.max}
                             onChange={e =>
                               handleChange(
                                 Max,
                                 ReactEvent.Form.target(e)##value,
                               )
                             }
                           />
                         </div>
                       </Div>
                     </Div>
                   </Div>
                   <Div flex={`num(3.)} />
                 </Div>
               </Antd.Form.Item>
             </>,
           )}
          {Primary.User.show(
             loggedUser,
             <Antd.Form.Item
               label={"Resolution Endpoint" |> Utils.ste}
               help={
                 "If you enter an url that returns a number, this will be called when the resolution date occurs, and entered as a judgement value."
                 |> Utils.ste
               }>
               <Input
                 value={state.values.resolutionEndpoint}
                 onChange={e =>
                   handleChange(
                     ResolutionEndpoint,
                     ReactEvent.Form.target(e)##value,
                   )
                 }
               />
             </Antd.Form.Item>,
           )}
          <Antd.Form.Item
            label={"Expected Resolution Date" |> Utils.ste}
            help={
              "When do you expect this will be resolvable by? You will get a notification when this date occurs."
              |> Utils.ste
            }>
            <DatePicker
              value={
                state.values.expectedResolutionDate
                |> MomentRe.momentDefaultFormat
              }
              onChange={e => {
                handleChange(ExpectedResolutionDate, e |> formatDate);

                (_ => ());
              }}
              disabled={state.values.showDescriptionDate == "TRUE"}
            />
          </Antd.Form.Item>
          {Primary.User.show(
             loggedUser,
             <Antd.Form.Item label={"Use Entities in Title" |> Utils.ste}>
               <Antd.Radio.Group
                 value={state.values.showDescriptionProperty}
                 defaultValue={state.values.showDescriptionProperty}
                 onChange={e =>
                   handleChange(
                     ShowDescriptionProperty,
                     ReactEvent.Form.target(e)##value,
                   )
                 }>
                 <Antd.Radio value="FALSE"> {"No" |> ste} </Antd.Radio>
                 <Antd.Radio value="TRUE">
                   {"Yes (Experimental)" |> ste}
                 </Antd.Radio>
               </Antd.Radio.Group>
             </Antd.Form.Item>,
           )}
          <Antd.Form.Item>
            <Button _type=`primary onClick={_ => onSubmit()}>
              {"Submit" |> ste}
            </Button>
          </Antd.Form.Item>
        </AntdForm>
      | _ => <Null />
      }
    }
  </Providers.AppContext.Consumer>;