open Rationale.Function.Infix;

open BsReform;

module FormConfig = [%lenses
  type state = {
    name: string,
    description: string,
    subjects: list(string),
    properties: list(string),
    dates: list(MomentRe.Moment.t),
  }
];

module Form = ReForm.Make(FormConfig);

let formatDate = E.M.format(E.M.format_standard);

let processArray =
  E.L.filter(r => r != "") ||> E.L.toArray ||> E.A.fmap(E.O.some);

module FormComponent = {
  [@react.component]
  let make =
      (
        ~reform: Form.api,
        ~result: ReasonApolloHooks.Mutation.controledVariantResult('a),
      ) => {
    let onSubmit = event => {
      ReactEvent.Synthetic.preventDefault(event);
      reform.submit();
    };

    <Form.Provider value=reform>
      {switch (result) {
       | Error(_error) => <p> {Lang.networkError |> Utils.ste} </p>
       | Data(_) => <p> {"Series are created." |> Utils.ste} </p>
       | _ =>
         <Antd.Form onSubmit>
           <h3>
             {"Warning: You can not edit a Series after created it at this time."
              |> Utils.ste}
           </h3>
           <Form.Field
             field=FormConfig.Name
             render={({handleChange, value}) =>
               <Antd.Form.Item label={"Name" |> Utils.ste}>
                 <Antd.Input
                   value
                   onChange={Helpers.handleChange(handleChange)}
                 />
               </Antd.Form.Item>
             }
           />
           <Form.Field
             field=FormConfig.Description
             render={({handleChange, value}) =>
               <Antd.Form.Item label={"Description" |> Utils.ste}>
                 <Antd.Input
                   value
                   onChange={Helpers.handleChange(handleChange)}
                 />
               </Antd.Form.Item>
             }
           />
           <Form.Field
             field=FormConfig.Subjects
             render={({handleChange, value}) =>
               <Antd.Form.Item label={"Subjects" |> Utils.ste}>
                 {value
                  |> E.L.fmapi((i, r) =>
                       <Antd.Input
                         value=r
                         onChange={e =>
                           handleChange(
                             value
                             |> E.L.update(
                                  ReactEvent.Form.target(e)##value,
                                  i,
                                ),
                           )
                         }
                       />
                     )
                  |> E.L.toArray
                  |> ReasonReact.array}
                 <Antd.Button
                   onClick={_ =>
                     value |> Rationale.RList.append("") |> handleChange
                   }>
                   {"Add Subject" |> Utils.ste}
                 </Antd.Button>
               </Antd.Form.Item>
             }
           />
           <Form.Field
             field=FormConfig.Properties
             render={({handleChange, value}) =>
               <Antd.Form.Item label={"Properties" |> Utils.ste}>
                 {value
                  |> E.L.fmapi((i, r) =>
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
                     )
                  |> E.L.toArray
                  |> ReasonReact.array}
                 <Antd.Button
                   onClick={_ =>
                     handleChange(value |> Rationale.RList.append(""))
                   }>
                   {"Add Property" |> Utils.ste}
                 </Antd.Button>
               </Antd.Form.Item>
             }
           />
           <Form.Field
             field=FormConfig.Dates
             render={({handleChange, value}) =>
               <Antd.Form.Item label={"Dates" |> Utils.ste}>
                 {value
                  |> E.L.fmapi((i, r) =>
                       <DatePicker
                         value=r
                         onChange={e => {
                           value |> E.L.update(e, i) |> handleChange;

                           (_ => ());
                         }}
                       />
                     )
                  |> E.L.toArray
                  |> ReasonReact.array}
                 <Antd.Button
                   onClick={_ =>
                     value
                     |> Rationale.RList.append(MomentRe.momentNow())
                     |> handleChange
                   }>
                   {"Add Date" |> Utils.ste}
                 </Antd.Button>
               </Antd.Form.Item>
             }
           />
           <Antd.Form.Item>
             {reform.state.formState == Submitting
                ? <Spin />
                : <Antd.Button _type=`primary onClick=onSubmit>
                    {"Submit" |> Utils.ste}
                  </Antd.Button>}
           </Antd.Form.Item>
         </Antd.Form>
       }}
    </Form.Provider>;
  };
};

module Create = {
  [@react.component]
  let make = (~channelId) => {
    let (mutate, result, _) = SeriesCreate.Mutation.use();

    let reform =
      Form.use(
        ~validationStrategy=OnDemand,
        ~schema=Form.Validation.Schema([||]),
        ~onSubmit=
          ({state}) => {
            mutate(
              ~variables=
                SeriesCreate.Query.make(
                  ~input={
                    "name": Some(state.values.name),
                    "description": Some(state.values.description),
                    "channelId": channelId,
                    "subjects": Some(state.values.subjects |> processArray),
                    "properties":
                      Some(state.values.properties |> processArray),
                    "dates":
                      Some(
                        state.values.dates
                        |> E.L.toArray
                        |> E.A.fmap(
                             formatDate ||> Js.Json.string ||> E.O.some,
                           ),
                      ),
                  },
                  (),
                )##variables,
              ~refetchQueries=[|"getChannels"|],
              (),
            )
            |> ignore;

            None;
          },
        ~initialState={
          description: "",
          name: "",
          subjects: [""],
          properties: [""],
          dates: [MomentRe.momentNow()],
        },
        (),
      );

    <FormComponent reform result />;
  };
};

[@react.component]
let make = (~channelId: string) => {
  <SLayout head={<SLayout.TextDiv text="Make a New Series" />}>
    <FC.PageCard.BodyPadding> <Create channelId /> </FC.PageCard.BodyPadding>
  </SLayout>;
};