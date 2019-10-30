[@bs.config {jsx: 3}];

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
          {"Communities" |> ReasonReact.string}
        </FC.PageCard.HeaderRow.Title>
      </FC.Base.Div>
    </div>;

  <SLayout head isFluid=false> body </SLayout>;
};
