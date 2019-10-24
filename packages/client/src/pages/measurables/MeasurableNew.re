[@bs.config {jsx: 3}];

module CMutationForm =
  MutationForm.Make({
    type queryType = MeasurableCreate.Query.t;
  });

let component = ReasonReact.statelessComponent("MeasurableNew");
let make = (~channelId, _children) => {
  ...component,
  render: _ => {
    let formCreator = mutation =>
      MeasurableForm.Form.make(
        ~schema=MeasurableForm.Form.Validation.Schema([||]),
        ~onSubmit=
          values =>
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
            ),
        ~initialState={
          name: "",
          labelCustom: "",
          labelProperty: "",
          labelSubject: "",
          expectedResolutionDate:
            MomentRe.momentNow() |> MeasurableForm.formatDate,
          labelOnDate: MomentRe.momentNow() |> MeasurableForm.formatDate,
          resolutionEndpoint: "",
          showDescriptionDate: "FALSE",
          showDescriptionProperty: "FALSE",
          valueType: "FLOAT",
          min: "",
          max: "",
          channelId,
        },
      );

    <SLayout head={SLayout.Header.textDiv("New Question")}>
      <FC.PageCard.BodyPadding>
        {MeasurableCreate.Mutation.make((mutation, data) =>
           formCreator(mutation, ({state, send, _}) =>
             CMutationForm.showWithLoading2(
               ~result=data.result,
               ~form=
                 MeasurableForm.showForm(
                   ~state,
                   ~send,
                   ~onSubmit=_ => send(MeasurableForm.Form.Submit),
                   (),
                 ),
               ~onSuccess=
                 (response: MeasurableCreate.Query.t) => {
                   switch (response##measurableCreate) {
                   | Some(m) =>
                     Routing.Url.push(MeasurableShow(channelId, m##id))
                   | _ => ()
                   };
                   ReasonReact.null;
                 },
               (),
             )
           )
           |> E.React.el
         )
         |> E.React.el}
      </FC.PageCard.BodyPadding>
    </SLayout>;
  },
};
