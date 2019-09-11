open Rationale.Function.Infix;

module CMutationForm =
  MutationForm.Make({
    type queryType = ChannelUpdate.Query.t;
  });

let component = ReasonReact.statelessComponent("ChannelEdit");

let make = (~channelId: string, ~loggedInUser: Types.user, _children) => {
  ...component,
  render: _ => {
    let loadChannel = ChannelGet.getChannelByIdAsComponent(~id=channelId);

    let mutationMake =
      ChannelUpdate.Mutation.make(~onCompleted=_ => ()) ||> E.React.el;

    let head =
      <>
        <Fc.FC.Base.Div float=`left>
          <Fc.FC.PageCard.HeaderRow.Title>
            {"Edit Community" |> Utils.ste}
          </Fc.FC.PageCard.HeaderRow.Title>
        </Fc.FC.Base.Div>
        <Fc.FC.Base.Div
          float=`right
          className={Css.style([
            Fc.FC.PageCard.HeaderRow.Styles.itemTopPadding,
            Fc.FC.PageCard.HeaderRow.Styles.itemBottomPadding,
          ])}>
          {Primary.User.show(
             loggedInUser,
             <Fc.FC.Base.Button
               variant=Primary
               onClick={e =>
                 LinkType.onClick(Internal(SeriesNew(channelId)), e)
               }>
               {"New Series" |> Utils.ste}
             </Fc.FC.Base.Button>,
           )}
        </Fc.FC.Base.Div>
      </>;

    let form = (mutation, channel: Types.channel) =>
      ChannelForm.Form.make(
        ~onSubmit=
          values =>
            ChannelUpdate.mutate(
              mutation,
              channelId,
              values.state.values.name,
              Some(values.state.values.description),
              values.state.values.isPublic |> E.Bool.fromString,
              values.state.values.isArchived |> E.Bool.fromString,
            ),
        ~initialState={
          name: channel.name,
          description: channel.description |> E.O.default(""),
          isPublic: channel.isPublic |> E.Bool.toString,
          isArchived: channel.isArchived |> E.Bool.toString,
        },
        ~schema=ChannelForm.Form.Validation.Schema([||]),
      )
      ||> E.React.el;

    <Fc.FC.PageCard.BodyPadding>
      {loadChannel(
         HttpResponse.fmap(channel =>
           mutationMake((mutation, data) =>
             form(mutation, channel, ({send, state}) =>
               CMutationForm.showWithLoading(
                 ~result=data.result,
                 ~form=
                   ChannelForm.showForm(
                     ~state,
                     ~send,
                     ~creating=false,
                     ~onSubmit=() => send(ChannelForm.Form.Submit),
                     (),
                   ),
                 ~successMessage="Community edited successfully.",
                 (),
               )
             )
           )
         )
         ||> HttpResponse.withReactDefaults,
       )}
    </Fc.FC.PageCard.BodyPadding>
    |> SLayout.LayoutConfig.make(~head, ~body=_)
    |> SLayout.FullPage.makeWithEl;
  },
};