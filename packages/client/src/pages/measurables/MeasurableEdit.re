module CMutationForm =
  MutationForm.Make({
    type queryType = MeasurableUpdate.Query.t;
  });

let formCreation = (id: string, m, loggedInUser: Types.user): React.element => {
  let measurable = MeasurableGet.toMeasurable(m);

  MeasurableUpdate.Mutation.make((mutation, data) =>
    MeasurableForm.MeasurableReForm.make(
      ~onSubmit=
        ({values}) =>
          MeasurableUpdate.mutate(
            mutation,
            id,
            values.name,
            values.labelCustom,
            values.expectedResolutionDate,
            values.resolutionEndpoint,
            values.labelSubject,
            values.labelOnDate,
            values.showDescriptionDate,
            values.labelProperty,
            values.valueType,
            values.min,
            values.max,
            values.channelId,
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
        resolutionEndpoint: measurable.resolutionEndpoint |> E.O.default(""),
        showDescriptionProperty: measurable.name == "" ? "TRUE" : "FALSE",
        labelProperty: measurable.labelProperty |> E.O.default(""),
        valueType: measurable.valueType |> Primary.Measurable.valueTypeToStr,
        min: measurable.min |> E.O.dimap(E.Float.toString, () => ""),
        max: measurable.max |> E.O.dimap(E.Float.toString, () => ""),
        channelId: measurable.channelId,
      },
      ~schema=[(`name, Custom(_ => None))],
      ({handleSubmit, handleChange, form, _}) =>
        CMutationForm.showWithLoading2(
          ~result=data.result,
          ~form=
            MeasurableForm.showForm(
              ~loggedInUser,
              ~form,
              ~handleSubmit,
              ~handleChange,
              ~creating=false,
              (),
            ),
          ~onSuccess=
            _ => {
              Routing.Url.push(MeasurableShow(measurable.channelId, id));
              ReasonReact.null;
            },
          (),
        ),
    )
    |> E.React.el
  )
  |> E.React.el;
};

let component = ReasonReact.statelessComponent("MeasurableEdit");

let make =
    (
      ~pageParams: PageConfig.LoggedInPage.pageParams,
      ~loggedInUser: Types.user,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _self =>
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Edit Question"),
      ~body=
        <FC.PageCard.BodyPadding>
          {MeasurableGet.component(~id=pageParams.id, m =>
             formCreation(pageParams.id, m, loggedInUser)
           )}
        </FC.PageCard.BodyPadding>,
    )
    |> layout,
};