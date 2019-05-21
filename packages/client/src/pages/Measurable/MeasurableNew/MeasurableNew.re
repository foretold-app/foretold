module SignUpForm = ReForm.Create(MeasurableForm.SignUpParams);

let component = ReasonReact.statelessComponent("MeasurableNew");

module CMutationForm =
  MutationForm.Make({
    type queryType = CreateMeasurableMutation.GraphQL.t;
  });

let make = (~channelId, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ => {
    let formCreator = mutation =>
      SignUpForm.make(
        ~onSubmit=({values}) => mutate(mutation, values, channelId),
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
        },
        ~schema=[(`name, Custom(_ => None))],
      );

    let head = SLayout.Header.textDiv("New Question");

    let body =
      CreateMeasurableMutation.Mutation.make(
        ~onCompleted=() => Context.Routing.Url.push(ChannelShow(channelId)),
        (mutation, data) =>
          formCreator(mutation, ({handleSubmit, handleChange, form, _}) =>
            CMutationForm.showWithLoading(
              ~result=data.result,
              ~form=
                MeasurableForm.showForm(~form, ~handleSubmit, ~handleChange),
              (),
            )
          )
          |> E.React.el,
      )
      |> E.React.el;

    SLayout.LayoutConfig.make(~head, ~body) |> layout;
  },
};