open Rationale.Function.Infix;

module CMutationForm =
  MutationForm.Make({
    type queryType = ChannelUpdate.Query.t;
  });

[@react.component]
let make = (~channelId: string) => {
  let loadChannel = ChannelGet.getChannelByIdAsComponent(~id=channelId);

  let head =
    <>
      <FC.Base.Div float=`left>
        <FC.PageCard.HeaderRow.Title>
          {"Edit Community" |> Utils.ste}
        </FC.PageCard.HeaderRow.Title>
      </FC.Base.Div>
      <Providers.AppContext.Consumer>
        ...{({loggedUser}) =>
          switch (loggedUser) {
          | Some(loggedUser) =>
            <FC.Base.Div
              float=`right
              className={Css.style([
                FC.PageCard.HeaderRow.Styles.itemTopPadding,
              ])}>
              {Primary.User.show(
                 loggedUser,
                 <FC.Base.Button
                   variant=FC.Base.Button.Primary
                   size=FC.Base.Button.Small
                   onClick={e =>
                     LinkType.onClick(Internal(SeriesNew(channelId)), e)
                   }>
                   {"New Series" |> Utils.ste}
                 </FC.Base.Button>,
               )}
            </FC.Base.Div>
          | _ => <Null />
          }
        }
      </Providers.AppContext.Consumer>
    </>;

  <SLayout head>
    <FC.PageCard.BodyPadding>
      {loadChannel(
         HttpResponse.fmap(channel =>
           <ChannelForm.Edit id=channelId channel />
         )
         ||> HttpResponse.withReactDefaults,
       )}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};