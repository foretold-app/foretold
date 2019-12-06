[@bs.config {jsx: 3}];

open Utils;
open Antd;

open BsReform;

open Style.Grid;

let formatDate = E.M.format(E.M.format_standard);

module Styles = {
  open Css;
  let shortInput = [width(`em(6.))] |> style;
};

module FormConfig = [%lenses
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
  }
];

module Form = ReForm.Make(FormConfig);

let showForm = (creating, reform: Form.api) =>
  <Providers.AppContext.Consumer>
    ...{({loggedUser}) =>
      switch (loggedUser) {
      | Some(loggedUser) =>
        <AntdForm
          onSubmit={event => {
            ReactEvent.Synthetic.preventDefault(event);
            reform.submit();
          }}>
          {E.React2.showIf(
             creating,
             <Form.Field
               field=FormConfig.ValueType
               render={({handleChange, error, value}) =>
                 <Antd.Form.Item
                   label={"Question Type" |> Utils.ste}
                   required=true
                   help={
                     "Number example: 'How many inches of rain will there be tomorrow?' Binary example: 'Will it rain tomorrow?'"
                     |> Utils.ste
                   }>
                   <Antd.Radio.Group
                     value
                     defaultValue=value
                     onChange={event =>
                       ReactEvent.Form.target(event)##value |> handleChange
                     }>
                     <Antd.Radio value="FLOAT"> {"Number" |> ste} </Antd.Radio>
                     <Antd.Radio value="PERCENTAGE">
                       {"Binary" |> ste}
                     </Antd.Radio>
                   </Antd.Radio.Group>
                 </Antd.Form.Item>
               }
             />,
           )}
          {E.React2.showIf(
             reform.state.values.showDescriptionProperty == "FALSE",
             <Form.Field
               field=FormConfig.Name
               render={({handleChange, error, value}) =>
                 <Antd.Form.Item
                   label={"Question Title" |> Utils.ste} required=true>
                   <Input
                     value
                     onChange={event =>
                       ReactEvent.Form.target(event)##value |> handleChange
                     }
                   />
                 </Antd.Form.Item>
               }
             />,
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
                        <Form.Field
                          field=FormConfig.ChannelId
                          render={({handleChange, error, value}) =>
                            <Antd.Form.Item label={"Community" |> Utils.ste}>
                              <Antd.Select
                                value onChange={e => handleChange(e)}>
                                c
                              </Antd.Select>
                            </Antd.Form.Item>
                          }
                        />
                    )
                  )
                )
             |> E.O.React.defaultNull,
           )}
          {E.React2.showIf(
             reform.state.values.showDescriptionProperty == "TRUE",
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
               <Form.Field
                 field=FormConfig.LabelSubject
                 render={({handleChange, error, value}) =>
                   <Antd.Form.Item
                     label={"Subject" |> Utils.ste} required=true>
                     <Antd.Input
                       value
                       onChange={event =>
                         ReactEvent.Form.target(event)##value |> handleChange
                       }
                     />
                   </Antd.Form.Item>
                 }
               />
               <Form.Field
                 field=FormConfig.LabelProperty
                 render={({handleChange, error, value}) =>
                   <Antd.Form.Item
                     label={"Property" |> Utils.ste} required=true>
                     <Antd.Input
                       value
                       onChange={event =>
                         ReactEvent.Form.target(event)##value |> handleChange
                       }
                     />
                   </Antd.Form.Item>
                 }
               />
               <Form.Field
                 field=FormConfig.ShowDescriptionDate
                 render={({handleChange, error, value}) =>
                   <Antd.Form.Item
                     label={"Include a Specific Date in Name" |> Utils.ste}>
                     <AntdSwitch
                       checked={value == "TRUE"}
                       onChange={e => handleChange(e ? "TRUE" : "FALSE")}
                     />
                   </Antd.Form.Item>
                 }
               />
               {reform.state.values.showDescriptionDate == "TRUE"
                  ? <Form.Field
                      field=FormConfig.LabelOnDate
                      render={({handleChange, error, value}) =>
                        <Antd.Form.Item label={"'On' Date" |> Utils.ste}>
                          <DatePicker
                            value={value |> MomentRe.moment}
                            onChange={e => {
                              handleChange(e |> formatDate);
                              //                          handleChange(
                              //                            ExpectedResolutionDate,
                              //                            e |> formatDate,
                              //                          );
                              _ => ();
                            }}
                          />
                        </Antd.Form.Item>
                      }
                    />
                  : <div />}
             </>,
           )}
          <Form.Field
            field=FormConfig.LabelCustom
            render={({handleChange, error, value}) =>
              <Antd.Form.Item
                label={"Description" |> Utils.ste}
                help={"Markdown supported." |> Utils.ste}>
                <Input.TextArea
                  style={ReactDOMRe.Style.make(~minHeight="12em", ())}
                  value
                  onChange={e =>
                    handleChange(ReactEvent.Form.target(e)##value)
                  }
                />
              </Antd.Form.Item>
            }
          />
          {E.React2.showIf(
             reform.state.values.valueType == "FLOAT",
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
                           <Form.Field
                             field=FormConfig.Min
                             render={({handleChange, error, value}) =>
                               <Antd.Input
                                 className=Styles.shortInput
                                 value
                                 onChange={e =>
                                   handleChange(
                                     ReactEvent.Form.target(e)##value,
                                   )
                                 }
                               />
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
                           <Form.Field
                             field=FormConfig.Max
                             render={({handleChange, error, value}) =>
                               <Antd.Input
                                 className=Styles.shortInput
                                 value
                                 onChange={e =>
                                   handleChange(
                                     ReactEvent.Form.target(e)##value,
                                   )
                                 }
                               />
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
               <Form.Field
                 field=FormConfig.ResolutionEndpoint
                 render={({handleChange, error, value}) =>
                   <Input
                     value
                     onChange={e =>
                       handleChange(ReactEvent.Form.target(e)##value)
                     }
                   />
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
            <Form.Field
              field=FormConfig.ResolutionEndpoint
              render={({handleChange, error, value}) =>
                <DatePicker
                  value={value |> MomentRe.momentDefaultFormat}
                  onChange={e => {
                    handleChange(e |> formatDate);

                    (_ => ());
                  }}
                  disabled={value == "TRUE"}
                />
              }
            />
          </Antd.Form.Item>
          {Primary.User.show(
             loggedUser,
             <Antd.Form.Item label={"Use Entities in Title" |> Utils.ste}>
               <Form.Field
                 field=FormConfig.ShowDescriptionProperty
                 render={({handleChange, error, value}) =>
                   <Antd.Radio.Group
                     value
                     defaultValue=value
                     onChange={e =>
                       handleChange(ReactEvent.Form.target(e)##value)
                     }>
                     <Antd.Radio value="FALSE"> {"No" |> ste} </Antd.Radio>
                     <Antd.Radio value="TRUE">
                       {"Yes (Experimental)" |> ste}
                     </Antd.Radio>
                   </Antd.Radio.Group>
                 }
               />
             </Antd.Form.Item>,
           )}
          <Antd.Form.Item>
            <Button
              _type=`primary
              onClick={event => {
                ReactEvent.Synthetic.preventDefault(event);
                reform.submit();
              }}>
              {"Submit" |> ste}
            </Button>
          </Antd.Form.Item>
        </AntdForm>
      | _ => <Null />
      }
    }
  </Providers.AppContext.Consumer>;