let component = ReasonReact.statelessComponent("MeasurableNew");

module CMutationForm =
  MutationForm.Make({
    type queryType = MeasurableCreate.Query.t;
  });

let make = (~channelId, ~loggedInUser: Types.user, _children) => {
  ...component,
  render: _ => {
    let formCreator = mutation =>
      MeasurableForm.MeasurableReForm.make(
        ~onSubmit=
          ({values}) =>
            MeasurableCreate.mutate(mutation, values, channelId),
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
          channelId: "",
        },
        ~schema=[(`name, Custom(_ => None))],
      );

    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("New Question"),
      ~body=
        <FC.PageCard.BodyPadding>
          {MeasurableCreate.Mutation.make((mutation, data) =>
             formCreator(mutation, ({handleSubmit, handleChange, form, _}) =>
               CMutationForm.showWithLoading2(
                 ~result=data.result,
                 ~form=
                   MeasurableForm.showForm(
                     ~loggedInUser,
                     ~form,
                     ~handleSubmit,
                     ~handleChange,
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
        </FC.PageCard.BodyPadding>,
    )
    |> SLayout.FullPage.makeWithEl;
  },
};