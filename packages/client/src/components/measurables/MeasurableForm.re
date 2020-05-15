open Rationale.Function.Infix;
open Utils;
open BsReform;

// @todo: To move these properties into the server-end:
// @todo: showDescriptionDate
// @todo: showDescriptionProperty

module Styles = {
  open Css;
  let shortInput = [width(`em(6.))] |> style;
  let labelSwitcher = [marginRight(`em(1.))] |> style;
};

module FormConfig = [%lenses
  type state = {
    name: string,
    labelCustom: string,
    labelSubject: string,
    labelOnDate: string,
    labelProperty: string,
    labelStartAtDate: string,
    labelEndAtDate: string,
    turnOnLabelStartAtDate: bool,
    turnOnLabelEndAtDate: bool,
    labelConditionals: list(string),
    expectedResolutionDate: string,
    resolutionEndpoint: string,
    showDescriptionDate: string,
    showDescriptionProperty: string,
    valueType: string,
    min: string,
    max: string,
    channelId: string,
    g: KenTools.g,
  }
];

module Form = ReForm.Make(FormConfig);

type result('a) = ReasonApolloHooks.Mutation.controledVariantResult('a);

let schema =
  Form.Validation.Schema([|
    Custom(
      Name,
      values =>
        values.showDescriptionProperty == "FALSE"
        && Js.String.length(values.name) > 512
          ? ReSchema.Error(Lang.less512) : Valid,
    ),
    Custom(
      Name,
      values =>
        values.showDescriptionProperty == "FALSE"
        && Js.String.length(values.name) < 3
          ? Error(Lang.more2) : Valid,
    ),
    Custom(
      LabelSubject,
      values =>
        values.showDescriptionProperty == "TRUE"
        && Js.String.length(values.labelSubject) > 512
          ? ReSchema.Error(Lang.less512) : Valid,
    ),
    Custom(
      LabelSubject,
      values =>
        values.showDescriptionProperty == "TRUE"
        && Js.String.length(values.labelSubject) < 3
          ? Error(Lang.more2) : Valid,
    ),
    Custom(
      LabelSubject,
      values =>
        values.showDescriptionProperty == "TRUE"
        && Js.String.length(
             KenTools.toString(values.g, values.labelSubject)
             |> E.O.default(""),
           )
        < 2
          ? Error(Lang.subjectLabelErr) : Valid,
    ),
    Custom(
      LabelProperty,
      values =>
        values.showDescriptionProperty == "TRUE"
        && Js.String.length(values.labelProperty) > 512
          ? ReSchema.Error(Lang.less512) : Valid,
    ),
    Custom(
      LabelProperty,
      values => {
        values.showDescriptionProperty == "TRUE"
        && Js.String.length(values.labelProperty) < 3
          ? Error(Lang.more2) : Valid
      },
    ),
    Custom(
      LabelProperty,
      values =>
        values.showDescriptionProperty == "TRUE"
        && Js.String.length(
             KenTools.toString(values.g, values.labelProperty)
             |> E.O.default(""),
           )
        < 2
          ? Error(Lang.propertyLabelErr) : Valid,
    ),
  |]);

let onSuccess = measurable => {
  switch (measurable) {
  | Some(measurable) =>
    setTimeout(
      _ =>
        Routing.Url.push(
          MeasurableShow(measurable##channelId, measurable##id),
        ),
      1000,
    )
    |> ignore;
    ();
  | _ => ()
  };
};

let formatDate = E.M.format(E.M.format_standard);

let momentToString = m =>
  m
  |> E.M.momentDefaultFormat
  |> MomentRe.Moment.startOf(`day)
  |> E.M.toJSON
  |> E.O.default("")
  |> Js.Json.string
  |> E.O.some;

let labelStartAtDate' = (values: FormConfig.state) => {
  values.turnOnLabelStartAtDate
    ? values.labelStartAtDate |> momentToString : Some(Js.Json.string(""));
};

let labelEndAtDate' = (values: FormConfig.state) =>
  values.turnOnLabelEndAtDate
    ? values.labelEndAtDate |> momentToString : Some(Js.Json.string(""));

let labelConditionals' = (values: FormConfig.state) => {
  values.labelConditionals
  |> E.L.filter(r => r != "")
  |> E.L.toArray
  |> E.A.fmap(Js.String.trim)
  |> E.A.fmap(Js.Json.string)
  |> E.O.some;
};

module FormComponent = {
  open Style.Grid;

  [@react.component]
  let make = (~creating: bool, ~reform: Form.api, ~result: result('a)) => {
    let g = KenTools.Graph.fromContext();
    let onSubmit = event => {
      ReactEvent.Synthetic.preventDefault(event);
      reform.submit();
    };

    <Form.Provider value=reform>
      {switch (result) {
       | Error(_error) => <Sorry />
       | Loading(_) => <Spin />
       | Data(_) => Lang.measurableIsSaved |> Utils.ste
       | _ =>
         let context = React.useContext(Providers.app);
         switch (context.loggedUser) {
         | Some(loggedUser) =>
           <Antd_Form onSubmit>
             {E.React2.showIf(
                creating,
                <Form.Field
                  field=FormConfig.ValueType
                  render={({handleChange, value}) =>
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
                        onChange={Helpers.handleChange(handleChange)}>
                        <Antd.Radio value="FLOAT">
                          {"Number" |> ste}
                        </Antd.Radio>
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
                  render={({handleChange, error, value, validate}) =>
                    <Antd.Form.Item
                      label={"Question Title" |> Utils.ste} required=true>
                      <Antd.Input
                        value
                        onBlur={_ => validate()}
                        onChange={Helpers.handleChange(handleChange)}
                      />
                      <Warning error />
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
                       ~order=ChannelsGet.orderSidebar,
                       channels =>
                       channels
                       |> E.A.fmapi((index, channel: Types.channel) =>
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
                             render={({handleChange, value}) =>
                               <Antd.Form.Item
                                 label={"Community" |> Utils.ste}>
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
                    render={({handleChange, value, error, validate}) =>
                      <Antd.Form.Item
                        label={"Subject" |> Utils.ste}
                        required=true
                        help={
                          KenTools.toString(
                            reform.state.values.g,
                            reform.state.values.labelSubject,
                          )
                          |> E.O.default("(none)")
                          |> Utils.ste
                        }>
                        <Antd.Input
                          value
                          onBlur={_ => validate()}
                          onChange={Helpers.handleChange(handleChange)}
                        />
                        <Warning error />
                      </Antd.Form.Item>
                    }
                  />
                  <Form.Field
                    field=FormConfig.LabelProperty
                    render={({handleChange, value, error, validate}) =>
                      <Antd.Form.Item
                        label={"Property" |> Utils.ste}
                        required=true
                        help={
                          KenTools.toString(
                            reform.state.values.g,
                            reform.state.values.labelProperty,
                          )
                          |> E.O.default("(none)")
                          |> Utils.ste
                        }>
                        <Antd.Input
                          value
                          onBlur={_ => validate()}
                          onChange={Helpers.handleChange(handleChange)}
                        />
                        <Warning error />
                      </Antd.Form.Item>
                    }
                  />
                  <Form.Field
                    field=FormConfig.ShowDescriptionDate
                    render={({handleChange, value}) =>
                      <Antd.Form.Item
                        label={"Include a Specific Date in Name" |> Utils.ste}>
                        <Antd_Switch
                          checked={value == "TRUE"}
                          onChange={e => handleChange(e ? "TRUE" : "FALSE")}
                        />
                      </Antd.Form.Item>
                    }
                  />
                  {reform.state.values.showDescriptionDate == "TRUE"
                     ? <Form.Field
                         field=FormConfig.LabelOnDate
                         render={({handleChange, value}) =>
                           <Antd.Form.Item label={"'On' Date" |> Utils.ste}>
                             <Antd_DatePicker
                               value={value |> MomentRe.moment}
                               onChange={e => {
                                 handleChange(e |> formatDate);
                                 _ => ();
                               }}
                             />
                           </Antd.Form.Item>
                         }
                       />
                     : <div />}
                </>,
              )}
             {E.React2.showIf(
                reform.state.values.showDescriptionProperty == "TRUE",
                <Experimental>
                  <Antd.Form.Item
                    help={
                      (
                        reform.state.values.turnOnLabelStartAtDate
                          ? "" : "Turn on start date with the swither."
                      )
                      |> Utils.ste
                    }
                    label={"Start Date" |> Utils.ste}>
                    <span className=Styles.labelSwitcher>
                      <Form.Field
                        field=FormConfig.TurnOnLabelStartAtDate
                        render={({handleChange, value}) =>
                          <Antd_Switch
                            checked=value
                            onChange=handleChange
                            size=`small
                          />
                        }
                      />
                    </span>
                    <Form.Field
                      field=FormConfig.LabelStartAtDate
                      render={({handleChange, value}) =>
                        <Antd_DatePicker
                          value={value |> MomentRe.momentDefaultFormat}
                          onChange={e => {
                            handleChange(e |> formatDate);
                            _ => ();
                          }}
                        />
                      }
                    />
                  </Antd.Form.Item>
                  <Antd.Form.Item
                    help={
                      (
                        reform.state.values.turnOnLabelEndAtDate
                          ? "" : "Turn on end date with the swither."
                      )
                      |> Utils.ste
                    }
                    label={"End Date" |> Utils.ste}>
                    <span className=Styles.labelSwitcher>
                      <Form.Field
                        field=FormConfig.TurnOnLabelEndAtDate
                        render={({handleChange, value}) =>
                          <Antd_Switch
                            checked=value
                            onChange=handleChange
                            size=`small
                          />
                        }
                      />
                    </span>
                    <Form.Field
                      field=FormConfig.LabelEndAtDate
                      render={({handleChange, value}) =>
                        <Antd_DatePicker
                          value={value |> MomentRe.momentDefaultFormat}
                          onChange={e => {
                            handleChange(e |> formatDate);
                            _ => ();
                          }}
                        />
                      }
                    />
                  </Antd.Form.Item>
                  <Form.Field
                    field=FormConfig.LabelConditionals
                    render={({handleChange, value}) =>
                      <Antd.Form.Item label={"Conditional On" |> Utils.ste}>
                        {value
                         |> E.L.fmapi((i, r) =>
                              <Antd.Form.Item
                                help={
                                  KenTools.toString(reform.state.values.g, r)
                                  |> E.O.default("(none)")
                                  |> Utils.ste
                                }>
                                <Antd.Input
                                  value=r
                                  onChange={e =>
                                    value
                                    |> E.L.update(
                                         ReactEvent.Form.target(e)##value,
                                         i,
                                       )
                                    |> handleChange
                                  }
                                />
                              </Antd.Form.Item>
                            )
                         |> E.L.toArray
                         |> ReasonReact.array}
                        <Antd.Button
                          onClick={_ =>
                            value
                            |> Rationale.RList.append("")
                            |> handleChange
                          }>
                          {"Add" |> Utils.ste}
                        </Antd.Button>
                      </Antd.Form.Item>
                    }
                  />
                </Experimental>,
              )}
             <Form.Field
               field=FormConfig.LabelCustom
               render={({handleChange, value}) =>
                 <Antd.Form.Item
                   label={"Description" |> Utils.ste}
                   help={"Markdown supported." |> Utils.ste}>
                   <Antd.Input.TextArea
                     style={ReactDOMRe.Style.make(~minHeight="12em", ())}
                     value
                     onChange={Helpers.handleChange(handleChange)}
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
                                render={({handleChange, value}) =>
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
                                render={({handleChange, value}) =>
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
             <Experimental>
               <Antd.Form.Item
                 label={"Resolution Endpoint" |> Utils.ste}
                 help={
                   "If you enter an url that returns a number, this will be called when the resolution date occurs, and entered as a judgement value."
                   |> Utils.ste
                 }>
                 <Form.Field
                   field=FormConfig.ResolutionEndpoint
                   render={({handleChange, value}) =>
                     <Antd.Input
                       value
                       onChange={Helpers.handleChange(handleChange)}
                     />
                   }
                 />
               </Antd.Form.Item>
             </Experimental>
             <Antd.Form.Item
               label={"Expected Resolution Date" |> Utils.ste}
               help={
                 "When do you expect this will be resolvable by? You will get a notification when this date occurs."
                 |> Utils.ste
               }>
               <Form.Field
                 field=FormConfig.ExpectedResolutionDate
                 render={({handleChange, value}) =>
                   <Antd_DatePicker
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
             <Experimental>
               <Antd.Form.Item label={"Use Entities in Title" |> Utils.ste}>
                 <Form.Field
                   field=FormConfig.ShowDescriptionProperty
                   render={({handleChange, value}) =>
                     <Antd.Radio.Group
                       value
                       defaultValue=value
                       onChange={Helpers.handleChange(handleChange)}>
                       <Antd.Radio value="FALSE"> {"No" |> ste} </Antd.Radio>
                       <Antd.Radio value="TRUE">
                         {"Yes (Experimental)" |> ste}
                       </Antd.Radio>
                     </Antd.Radio.Group>
                   }
                 />
               </Antd.Form.Item>
             </Experimental>
             <Antd.Form.Item>
               {reform.state.formState == Submitting
                  ? <Spin />
                  : <Antd.Button _type=`primary onClick=onSubmit>
                      {"Submit" |> ste}
                    </Antd.Button>}
             </Antd.Form.Item>
           </Antd_Form>
         | _ => <Null />
         };
       }}
    </Form.Provider>;
  };
};

// @todo: The "refetchQueries" policy does not work
// @todo: since components to update are destroyed in the DOM
// @todo: at a moment of refetching.

module Create = {
  let onSuccess' = response => onSuccess(response##measurableCreate);

  [@react.component]
  let make = (~channelId: string) => {
    let g = KenTools.Graph.fromContext();
    let (mutate, result, _) = MeasurableCreate.Mutation.use();

    let reform =
      Form.use(
        ~validationStrategy=OnDemand,
        ~schema,
        ~onSubmit=
          ({state: {values}}) => {
            let labelStartAtDate = labelStartAtDate'(values);
            let labelEndAtDate = labelEndAtDate'(values);
            let labelConditionals = labelConditionals'(values);

            let input =
              values.showDescriptionDate == "TRUE"
                ? {
                  "name": values.name |> E.J.fromString,
                  "labelCustom": Some(values.labelCustom),
                  "labelProperty": Some(values.labelProperty),
                  "expectedResolutionDate":
                    values.expectedResolutionDate |> Js.Json.string |> E.O.some,
                  "resolutionEndpoint": values.resolutionEndpoint |> E.O.some,
                  "labelSubject": values.labelSubject |> E.O.some,
                  "labelOnDate":
                    values.labelOnDate |> Js.Json.string |> E.O.some,
                  "valueType": `FLOAT,
                  "channelId": values.channelId,
                  "min":
                    values.min == ""
                      ? None : Some(values.min |> float_of_string),
                  "max":
                    values.max == ""
                      ? None : Some(values.max |> float_of_string),
                  "labelStartAtDate": labelStartAtDate,
                  "labelEndAtDate": labelEndAtDate,
                  "labelConditionals": labelConditionals,
                }
                : {
                  "name": values.name |> E.J.fromString,
                  "labelCustom": Some(values.labelCustom),
                  "labelProperty": Some(values.labelProperty),
                  "expectedResolutionDate":
                    values.expectedResolutionDate |> Js.Json.string |> E.O.some,
                  "resolutionEndpoint": values.resolutionEndpoint |> E.O.some,
                  "labelSubject": values.labelSubject |> E.O.some,
                  "labelOnDate": None,
                  "valueType":
                    values.valueType |> Primary.Measurable.valueTypeToEnum,
                  "channelId": values.channelId,
                  "min":
                    values.min == ""
                      ? None : Some(values.min |> float_of_string),
                  "max":
                    values.max == ""
                      ? None : Some(values.max |> float_of_string),
                  "labelStartAtDate": labelStartAtDate,
                  "labelEndAtDate": labelEndAtDate,
                  "labelConditionals": labelConditionals,
                };

            mutate(
              ~variables=MeasurableCreate.Query.make(~input, ())##variables,
              ~refetchQueries=[|"agent", "measurable", "measurements"|],
              (),
            )
            |> Js.Promise.then_((result: result('a)) => {
                 switch (result) {
                 | Data(data) => onSuccess'(data)
                 | _ => ()
                 };
                 Js.Promise.resolve();
               })
            |> ignore;

            None;
          },
        ~initialState={
          name: "",
          labelCustom: "",
          labelProperty: "",
          labelSubject: "",
          expectedResolutionDate:
            MomentRe.momentNow()
            |> MomentRe.Moment.add(
                 ~duration=MomentRe.duration(1.0, `months),
               )
            |> formatDate,
          labelOnDate: MomentRe.momentNow() |> formatDate,
          labelStartAtDate: MomentRe.momentNow() |> formatDate,
          labelEndAtDate:
            MomentRe.momentNow()
            |> MomentRe.Moment.add(
                 ~duration=MomentRe.duration(1.0, `months),
               )
            |> formatDate,
          labelConditionals: [""],
          resolutionEndpoint: "",
          showDescriptionDate: "FALSE",
          showDescriptionProperty: "FALSE",
          valueType: "FLOAT",
          min: "",
          max: "",
          channelId,
          turnOnLabelStartAtDate: false,
          turnOnLabelEndAtDate: false,
          g,
        },
        (),
      );

    <FormComponent creating=true reform result />;
  };
};

module Edit = {
  [@react.component]
  let make = (~id, ~measurable: Types.measurable) => {
    let g = KenTools.Graph.fromContext();
    let (mutate, result, _) = MeasurableUpdate.Mutation.use();

    let reform =
      Form.use(
        ~validationStrategy=OnDemand,
        ~schema,
        ~onSubmit=
          ({state: {values}}) => {
            let labelStartAtDate = labelStartAtDate'(values);
            let labelEndAtDate = labelEndAtDate'(values);
            let labelConditionals = labelConditionals'(values);

            let date =
              values.showDescriptionDate == "TRUE" ? values.labelOnDate : "";
            let expectedResolutionDate =
              values.expectedResolutionDate |> momentToString;

            let input = {
              "name": values.name |> E.J.fromString,
              "labelCustom": values.labelCustom |> E.O.some,
              "labelProperty": values.labelProperty |> E.O.some,
              "labelOnDate": date |> Js.Json.string |> E.O.some,
              "expectedResolutionDate": expectedResolutionDate,
              "resolutionEndpoint": values.resolutionEndpoint |> E.O.some,
              "labelSubject": values.labelSubject |> E.O.some,
              "valueType":
                values.valueType |> Primary.Measurable.valueTypeToEnum,
              "min":
                values.min != ""
                  ? values.min |> Js.Float.fromString |> E.O.some : None,
              "max":
                values.max != ""
                  ? values.max |> Js.Float.fromString |> E.O.some : None,
              "channelId": values.channelId,
              "labelStartAtDate": labelStartAtDate,
              "labelEndAtDate": labelEndAtDate,
              "labelConditionals": labelConditionals,
            };

            mutate(
              ~variables=
                MeasurableUpdate.Query.make(~id, ~input, ())##variables,
              ~refetchQueries=[|"measurables", "measurable"|],
              (),
            )
            |> ignore;

            None;
          },
        ~initialState={
          name: measurable.name,
          labelOnDate:
            measurable.labelOnDate
            |> E.O.default(MomentRe.momentNow())
            |> formatDate,
          showDescriptionDate:
            measurable.labelOnDate |> E.O.isSome |> (e => e ? "TRUE" : "FALSE"),
          labelSubject: measurable.labelSubject |> E.O.default(""),
          labelCustom: measurable.labelCustom |> E.O.default(""),
          labelStartAtDate:
            measurable.labelStartAtDate
            |> E.O.default(MomentRe.momentNow())
            |> formatDate,
          labelEndAtDate:
            measurable.labelEndAtDate
            |> E.O.default(MomentRe.momentNow())
            |> formatDate,
          labelConditionals:
            measurable.labelConditionals
            |> E.O.fmap(E.A.to_list)
            |> E.O.default([""]),
          expectedResolutionDate:
            measurable.expectedResolutionDate
            |> E.O.default(MomentRe.momentNow())
            |> formatDate,
          resolutionEndpoint:
            measurable.resolutionEndpoint |> E.O.default(""),
          showDescriptionProperty: measurable.name == "" ? "TRUE" : "FALSE",
          labelProperty: measurable.labelProperty |> E.O.default(""),
          valueType: measurable.valueType |> Primary.Measurable.valueTypeToStr,
          min: measurable.min |> E.O.dimap(E.Float.toString, () => ""),
          max: measurable.max |> E.O.dimap(E.Float.toString, () => ""),
          channelId: measurable.channelId,
          turnOnLabelStartAtDate: measurable.labelStartAtDate |> E.O.toBool,
          turnOnLabelEndAtDate: measurable.labelEndAtDate |> E.O.toBool,
          g,
        },
        (),
      );

    <FormComponent creating=false reform result />;
  };
};