module CMutationForm =
  MutationForm.Make({
    type queryType = MeasurableUpdate.GraphQL.t;
  });

let formCreation = (id: string, m: MeasurableGet.measurable): React.element => {
  let measurable: Primary.Measurable.t = MeasurableGet.toMeasurable(m);

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
        min: measurable.min |> E.O.dimap(Js.Float.toString, () => ""),
        max: measurable.max |> E.O.dimap(Js.Float.toString, () => ""),
      },
      ~schema=[(`name, Custom(_ => None))],
      ({handleSubmit, handleChange, form, _}) =>
        CMutationForm.showWithLoading2(
          ~result=data.result,
          ~form=MeasurableForm.showForm(~form, ~handleSubmit, ~handleChange),
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
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _self =>
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Edit Question"),
      ~body=
        <FC.PageCard.BodyPadding>
          {MeasurableGet.component(
             ~id=pageParams.id, (m: MeasurableGet.measurable) =>
             formCreation(pageParams.id, m)
           )}
        </FC.PageCard.BodyPadding>,
    )
    |> layout,
};