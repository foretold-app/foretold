[@react.component]
let make = (~channelId: string) => {
  let addToChannelLink = (agentId: string, channelId: string) =>
    <ChannelMembershipCreate.Mutation>
      ...{(mutation, result: ChannelMembershipCreate.Mutation.renderPropObj) =>
        <Antd.Button
          loading={Bool(result.loading)}
          onClick={_ =>
            ChannelMembershipCreate.mutate(mutation, agentId, channelId)
          }>
          {"Add to Community" |> Utils.ste}
        </Antd.Button>
      }
    </ChannelMembershipCreate.Mutation>;

  let agent =
    Table.Column.make(
      ~name="Member" |> Utils.ste,
      ~render=(agent: Types.agent) => <AgentLink agent />,
      (),
    );

  let invite =
    Table.Column.make(
      ~name="Invite" |> Utils.ste,
      ~render=(agent: Types.agent) => addToChannelLink(agent.id, channelId),
      (),
    );

  let all = [|agent, invite|];

  let title =
    <ForetoldComponents.Base.Div float=`left>
      <ForetoldComponents.PageCard.HeaderRow.Title>
        {"Add Agents" |> Utils.ste}
      </ForetoldComponents.PageCard.HeaderRow.Title>
    </ForetoldComponents.Base.Div>;

  let onSuccess = agents => {
    let dataSource =
      agents
      |> Js.Array.filter((agent: Types.agent) =>
           switch (agent.name) {
           | Some(name) when name != "" => true
           | _ => false
           }
         );

    <Table columns=all rows=dataSource />;
  };

  let onError = e => <Error e />;

  let loadingFn = () => <Spin />;

  let table =
    AgentsGet.componentUsers(~excludeChannelId=channelId, agents =>
      agents |> HttpResponse.flatten(onSuccess, onError, loadingFn)
    );

  <SLayout head={<div> title </div>}>
    <ForetoldComponents.PageCard.Body>
      table
    </ForetoldComponents.PageCard.Body>
  </SLayout>;
};