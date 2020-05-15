open Rationale.Function.Infix;

[@react.component]
let make = (~channelId: string) => {
  let loadChannel = ChannelGet.getChannelByIdAsComponent(~id=channelId);
  let head =
    <>
      <ForetoldComponents.Base.Div float=`left>
        <ForetoldComponents.PageCard.HeaderRow.Title>
          {"Edit Community" |> Utils.ste}
        </ForetoldComponents.PageCard.HeaderRow.Title>
      </ForetoldComponents.Base.Div>
      <Experimental>
        <ForetoldComponents.Base.Div
          float=`right
          className={Css.style([
            ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
          ])}>
          <ForetoldComponents.Base.Button
            variant=ForetoldComponents.Base.Button.Primary
            size=ForetoldComponents.Base.Button.Small
            onClick={e =>
              LinkType.onClick(Internal(SeriesNew(channelId)), e)
            }>
            {"New Series" |> Utils.ste}
          </ForetoldComponents.Base.Button>
        </ForetoldComponents.Base.Div>
      </Experimental>
    </>;

  <SLayout head>
    <ForetoldComponents.PageCard.BodyPadding>
      {loadChannel(
         HttpResponse.fmap(channel =>
           <ChannelForm.Edit id=channelId channel />
         )
         ||> HttpResponse.withReactDefaults,
       )}
    </ForetoldComponents.PageCard.BodyPadding>
  </SLayout>;
};