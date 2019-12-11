type column = Table.column(Types.agent);

[@react.component]
let make = (~channelId: string) => {
  let addToChannelLink = (agentId: string, channelId: string) =>
    <ChannelMembershipCreate.Mutation>
      ...{(mutation, _) =>
        <Link
          linkType={
            Action(
              _ =>
                ChannelMembershipCreate.mutate(mutation, agentId, channelId),
            )
          }>
          {"Add to Community" |> Utils.ste}
        </Link>
      }
    </ChannelMembershipCreate.Mutation>;

  let agentColumn =
    Table.Column.make(
      ~name="Member" |> Utils.ste,
      ~render=(agent: Types.agent) => <AgentLink agent />,
      (),
    );

  let inviteColumn =
    Table.Column.make(
      ~name="Invite" |> Utils.ste,
      ~render=(agent: Types.agent) => addToChannelLink(agent.id, channelId),
      (),
    );

  let all: array(column) = [|agentColumn, inviteColumn|];

  let title =
    <FC.Base.Div float=`left>
      <FC.PageCard.HeaderRow.Title>
        {"Add Agents" |> Utils.ste}
      </FC.PageCard.HeaderRow.Title>
    </FC.Base.Div>;

  let onSuccess = agents => {
    let dataSource =
      agents
      |> Js.Array.filter((agent: Types.agent) =>
           switch (agent.name) {
           | Some(name) when name != "" => true
           | _ => false
           }
         );

    Table.fromColumns(all, dataSource, ());
  };

  let onError = e => <Error e />;

  let loadingFn = () => <Spin />;

  let table =
    AgentsGet.componentUsers(~excludeChannelId=channelId, agents =>
      agents |> HttpResponse.flatten(onSuccess, onError, loadingFn)
    );

  <SLayout head={<div> title </div>}>
    <FC.PageCard.Body> table </FC.PageCard.Body>
  </SLayout>;
};