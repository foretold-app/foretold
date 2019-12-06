[@bs.config {jsx: 3}];

module CMutationForm =
  MutationForm.Make({
    type queryType = MeasurableCreate.Query.t;
  });

[@react.component]
let make = (~channelId) => {
  let formCreator = mutation =>
    MeasurableForm.Form.use(
      ~schema=MeasurableForm.Form.Validation.Schema([||]),
      ~onSubmit=
        values => {
          MeasurableCreate.mutate(
            mutation,
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
          );
          None;
        },
      ~initialState={
        name: "",
        labelCustom: "",
        labelProperty: "",
        labelSubject: "",
        expectedResolutionDate:
          MomentRe.momentNow()
          |> MomentRe.Moment.add(~duration=MomentRe.duration(1.0, `months))
          |> MeasurableForm.formatDate,
        labelOnDate: MomentRe.momentNow() |> MeasurableForm.formatDate,
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

  <SLayout head={SLayout.Header.textDiv("New Question")}>
    <FC.PageCard.BodyPadding>
      {<MeasurableCreate.Mutation>
         ...{(mutation, data) => {
           let reform = formCreator(mutation);

           <MeasurableForm.Form.Provider value=reform>
             {CMutationForm.showWithLoading2(
                ~result=data.result,
                ~form=MeasurableForm.showForm(true, reform),
                ~onSuccess=
                  (response: MeasurableCreate.Query.t) => {
                    switch (response##measurableCreate) {
                    | Some(m) =>
                      Routing.Url.push(MeasurableShow(channelId, m##id))
                    | _ => ()
                    };
                    <Null />;
                  },
                (),
              )}
           </MeasurableForm.Form.Provider>;
         }}
       </MeasurableCreate.Mutation>}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};