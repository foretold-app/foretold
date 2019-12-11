[@react.component]
let make = (~agentId: string) => {
  let body =
    <FC.PageCard.Body>
      <ChannelTable agentId={Some(agentId)} />
    </FC.PageCard.Body>;

  let head =
    <div>
      <FC.Base.Div float=`left>
        <FC.PageCard.HeaderRow.Title>
          {"Communities" |> Utils.ste}
        </FC.PageCard.HeaderRow.Title>
      </FC.Base.Div>
    </div>;

  <SLayout head isFluid=false> body </SLayout>;
};