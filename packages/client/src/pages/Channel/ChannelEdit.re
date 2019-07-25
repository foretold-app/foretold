open Rationale.Function.Infix;

module ChannelFormShower = ReForm.Create(ChannelForm.NewChannelParams);

module CMutationForm =
  MutationForm.Make({
    type queryType = ChannelUpdate.Query.t;
  });

let component = ReasonReact.statelessComponent("ChannelNew");

let make = (~channelId: string, ~layout, _children) => {
  ...component,
  render: _ => {
    let loadChannel = ChannelGet.getChannelByIdAsComponent(~id=channelId);

    let mutationMake =
      ChannelUpdate.Mutation.make(~onCompleted=_ => ()) ||> E.React.el;

    let header =
      <>
        <FC.Base.Div float=`left>
          <FC.PageCard.HeaderRow.Title>
            {"Edit Community" |> Utils.ste}
          </FC.PageCard.HeaderRow.Title>
        </FC.Base.Div>
        <FC.Base.Div
          float=`right
          className={Css.style([
            FC.PageCard.HeaderRow.Styles.itemTopPadding,
            FC.PageCard.HeaderRow.Styles.itemBottomPadding,
          ])}>
          <FC.Base.Button
            variant=Primary
            onClick={e =>
              LinkType.onClick(Internal(SeriesNew(channelId)), e)
            }>
            {"New Series" |> Utils.ste}
          </FC.Base.Button>
        </FC.Base.Div>
      </>;

    let form = (mutation, channel: Primary.Channel.t) =>
      ChannelFormShower.make(
        ~onSubmit=
          ({values}) =>
            ChannelUpdate.mutate(
              mutation,
              channelId,
              values.name,
              Some(values.description),
              values.isPublic |> E.Bool.fromString,
            ),
        ~initialState={
          name: channel.name,
          description: channel.description |> E.O.default(""),
          isPublic: channel.isPublic |> E.Bool.toString,
        },
        ~schema=[(`name, Custom(_ => None))],
      )
      ||> E.React.el;

    <FC.PageCard.BodyPadding>
      {loadChannel(
         HttpResponse.fmap(result =>
           mutationMake((mutation, data) =>
             form(mutation, result, ({handleSubmit, handleChange, form, _}) =>
               CMutationForm.showWithLoading(
                 ~result=data.result,
                 ~form=
                   ChannelForm.showForm(~form, ~handleSubmit, ~handleChange),
                 ~successMessage="Community edited successfully.",
                 (),
               )
             )
           )
         )
         ||> HttpResponse.withReactDefaults,
       )}
    </FC.PageCard.BodyPadding>
    |> SLayout.LayoutConfig.make(~head=header, ~body=_)
    |> layout;
  },
};