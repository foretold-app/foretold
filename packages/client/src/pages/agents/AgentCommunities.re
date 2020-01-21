[@react.component]
let make = (~agentId: string) => {
  let body =
    <ForetoldComponents.PageCard.Body>
      <ChannelTable agentId={Some(agentId)} />
    </ForetoldComponents.PageCard.Body>;

  let head =
    <div>
      <ForetoldComponents.Base.Div float=`left>
        <ForetoldComponents.PageCard.HeaderRow.Title>
          {"Communities" |> Utils.ste}
        </ForetoldComponents.PageCard.HeaderRow.Title>
      </ForetoldComponents.Base.Div>
    </div>;

  <SLayout head> body </SLayout>;
};