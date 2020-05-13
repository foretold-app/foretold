open Utils;
open BsReform;

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

type result('a) = ReasonApolloHooks.Mutation.controledVariantResult('a);

let formatDate = E.M.format(E.M.format_standard);

let schema =
  Form.Validation.Schema([|
    Custom(
      Name,
      values =>
        Js.String.length(values.name) > 512
          ? ReSchema.Error("Must be less than 512 characters.") : Valid,
    ),
    Custom(
      Name,
      values =>
        Js.String.length(values.name) < 3
          ? Error("Must be over 2 characters.") : Valid,
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

module FormComponent = {
  open Style.Grid;

  [@react.component]
  let make = (~creating: bool, ~reform: Form.api, ~result: result('a)) => {
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
                    render={({handleChange, value}) =>
                      <Antd.Form.Item
                        label={"Subject" |> Utils.ste} required=true>
                        <Antd.Input
                          value
                          onChange={Helpers.handleChange(handleChange)}
                        />
                      </Antd.Form.Item>
                    }
                  />
                  <Form.Field
                    field=FormConfig.LabelProperty
                    render={({handleChange, value}) =>
                      <Antd.Form.Item
                        label={"Property" |> Utils.ste} required=true>
                        <Antd.Input
                          value
                          onChange={Helpers.handleChange(handleChange)}
                        />
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
                    render={({handleChange, value}) =>
                      <Antd.Input
                        value
                        onChange={Helpers.handleChange(handleChange)}
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
             {Primary.User.show(
                loggedUser,
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
                </Antd.Form.Item>,
              )}
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
    let (mutate, result, _) = MeasurableCreate.Mutation.use();

    let reform =
      Form.use(
        ~validationStrategy=OnDemand,
        ~schema,
        ~onSubmit=
          ({state}) => {
            let input =
              state.values.showDescriptionDate == "TRUE"
                ? {
                  "name": state.values.name |> E.J.fromString,
                  "labelCustom": Some(state.values.labelCustom),
                  "labelProperty": Some(state.values.labelProperty),
                  "expectedResolutionDate":
                    state.values.expectedResolutionDate
                    |> Js.Json.string
                    |> E.O.some,
                  "resolutionEndpoint":
                    state.values.resolutionEndpoint |> E.O.some,
                  "labelSubject": state.values.labelSubject |> E.O.some,
                  "labelOnDate":
                    state.values.labelOnDate |> Js.Json.string |> E.O.some,
                  "valueType": `FLOAT,
                  "channelId": state.values.channelId,
                  "min":
                    state.values.min == ""
                      ? None : Some(state.values.min |> float_of_string),
                  "max":
                    state.values.max == ""
                      ? None : Some(state.values.max |> float_of_string),
                  "labelStartAtDate": None,
                  "labelEndAtDate": None,
                  "labelConditionals": None,
                }
                : {
                  "name": state.values.name |> E.J.fromString,
                  "labelCustom": Some(state.values.labelCustom),
                  "labelProperty": Some(state.values.labelProperty),
                  "expectedResolutionDate":
                    state.values.expectedResolutionDate
                    |> Js.Json.string
                    |> E.O.some,
                  "resolutionEndpoint":
                    state.values.resolutionEndpoint |> E.O.some,
                  "labelSubject": state.values.labelSubject |> E.O.some,
                  "labelOnDate": None,
                  "valueType":
                    state.values.valueType
                    |> Primary.Measurable.valueTypeToEnum,
                  "channelId": state.values.channelId,
                  "min":
                    state.values.min == ""
                      ? None : Some(state.values.min |> float_of_string),
                  "max":
                    state.values.max == ""
                      ? None : Some(state.values.max |> float_of_string),
                  "labelStartAtDate": None,
                  "labelEndAtDate": None,
                  "labelConditionals": None,
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
          resolutionEndpoint: "",
          showDescriptionDate: "FALSE",
          showDescriptionProperty: "FALSE",
          valueType: "FLOAT",
          min: "",
          max: "",
          channelId,
        },
        (),
      );

    <FormComponent creating=true reform result />;
  };
};

module Edit = {
  [@react.component]
  let make = (~id, ~measurable: Types.measurable) => {
    let (mutate, result, _) = MeasurableUpdate.Mutation.use();

    let reform =
      Form.use(
        ~validationStrategy=OnDemand,
        ~schema,
        ~onSubmit=
          ({state}) => {
            let date =
              state.values.showDescriptionDate == "TRUE"
                ? state.values.labelOnDate : "";

            let expectedResolutionDate =
              state.values.expectedResolutionDate
              |> E.M.momentDefaultFormat
              |> E.M.toJSON
              |> E.O.default("")
              |> Js.Json.string
              |> Rationale.Option.some;

            mutate(
              ~variables=
                MeasurableUpdate.Query.make(
                  ~id,
                  ~input={
                    "name": state.values.name |> E.J.fromString,
                    "labelCustom":
                      state.values.labelCustom |> Rationale.Option.some,
                    "labelProperty":
                      state.values.labelProperty |> Rationale.Option.some,
                    "labelOnDate":
                      date |> Js.Json.string |> Rationale.Option.some,
                    "expectedResolutionDate": expectedResolutionDate,
                    "resolutionEndpoint":
                      state.values.resolutionEndpoint |> Rationale.Option.some,
                    "labelSubject":
                      state.values.labelSubject |> Rationale.Option.some,
                    "valueType":
                      state.values.valueType
                      |> Primary.Measurable.valueTypeToEnum,
                    "min":
                      state.values.min != ""
                        ? state.values.min
                          |> Js.Float.fromString
                          |> Rationale.Option.some
                        : None,
                    "max":
                      state.values.max != ""
                        ? state.values.max
                          |> Js.Float.fromString
                          |> Rationale.Option.some
                        : None,
                    "channelId": state.values.channelId,
                    "labelStartAtDate": None,
                    "labelEndAtDate": None,
                    "labelConditionals": None,
                  },
                  (),
                )##variables,
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
        },
        (),
      );

    <FormComponent creating=false reform result />;
  };
};