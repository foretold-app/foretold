[@bs.config {jsx: 3}];

module CMutationForm =
  MutationForm.Make({
    type queryType = MeasurableUpdate.Query.t;
  });

let formCreation = (id: string, measurable: Types.measurable) => {
  <MeasurableUpdate.Mutation>
    ...{(mutation, data) =>
      MeasurableForm.Form.make(
        ~schema=MeasurableForm.Form.Validation.Schema([||]),
        ~onSubmit=
          values =>
            MeasurableUpdate.mutate(
              mutation,
              id,
              values.state.values.name,
              values.state.values.labelCustom,
              values.state.values.expectedResolutionDate,
              values.state.values.resolutionEndpoint,
              values.state.values.labelSubject,
              values.state.values.labelOnDate,
              values.state.values.showDescriptionDate,
              values.state.values.labelProperty,
              values.state.values.valueType,
              values.state.values.min,
              values.state.values.max,
              values.state.values.channelId,
            ),
        ~initialState={
          name: measurable.name,
          labelOnDate:
            measurable.labelOnDate
            |> E.O.default(MomentRe.momentNow())
            |> MeasurableForm.formatDate,
          showDescriptionDate:
            measurable.labelOnDate |> E.O.isSome |> (e => e ? "TRUE" : "FALSE"),
          labelSubject: measurable.labelSubject |> E.O.default(""),
          labelCustom: measurable.labelCustom |> E.O.default(""),
          expectedResolutionDate:
            measurable.expectedResolutionDate
            |> E.O.default(MomentRe.momentNow())
            |> MeasurableForm.formatDate,
          resolutionEndpoint:
            measurable.resolutionEndpoint |> E.O.default(""),
          showDescriptionProperty: measurable.name == "" ? "TRUE" : "FALSE",
          labelProperty: measurable.labelProperty |> E.O.default(""),
          valueType: measurable.valueType |> Primary.Measurable.valueTypeToStr,
          min: measurable.min |> E.O.dimap(E.Float.toString, () => ""),
          max: measurable.max |> E.O.dimap(E.Float.toString, () => ""),
          channelId: measurable.channelId,
        },
        ({state, send, _}) =>
          CMutationForm.showWithLoading2(
            ~result=data.result,
            ~form=
              MeasurableForm.showForm(
                ~state,
                ~send,
                ~creating=false,
                ~onSubmit=_ => send(MeasurableForm.Form.Submit),
                (),
              ),
            ~onSuccess=
              (response: MeasurableUpdate.Query.t) => {
                switch (response##measurableUpdate) {
                | Some(measurable) =>
                  Routing.Url.push(MeasurableShow(measurable##channelId, id))
                | _ => ()
                };
                ReasonReact.null;
              },
            (),
          ),
      )
      |> E.React.el
    }
  </MeasurableUpdate.Mutation>;
};

[@react.component]
let make = (~pageParams: Types.pageParams) => {
  <SLayout head={SLayout.Header.textDiv("Edit Question")}>
    <FC.PageCard.BodyPadding>
      {MeasurableGet.component(~id=pageParams.id, m =>
         formCreation(pageParams.id, m)
       )}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};
