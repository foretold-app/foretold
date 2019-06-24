let component = ReasonReact.statelessComponent("ChannelMembers");

type column = Table.column(Context.Primary.Agent.t);

let make =
    (
      ~channelId: string,
      ~loggedInUser: Context.Primary.User.t,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ => {
    let addToChannelLink = (agentId: string, channelId: string) =>
      Foretold__GraphQL.Mutations.ChannelMembershipCreate.Mutation.make(
        (mutation, _) =>
        <Foretold__Components__Link
          linkType={
            Action(
              _ =>
                Foretold__GraphQL.Mutations.ChannelMembershipCreate.mutate(
                  mutation,
                  agentId,
                  channelId,
                ),
            )
          }>
          {"Add to Community" |> ReasonReact.string}
        </Foretold__Components__Link>
      )
      |> ReasonReact.element;

    let agentColumn: column = {
      name: "Agent" |> ReasonReact.string,
      render: (agent: Context.Primary.Agent.t) =>
        <Foretold__Components__Link
          linkType={
            Internal(Agent({agentId: agent.id, subPage: AgentMeasurements}))
          }>
          {agent.name |> ReasonReact.string}
        </Foretold__Components__Link>,
      flex: 1,
    };

    let inviteColumn: column = {
      name: "Invite" |> ReasonReact.string,
      render: (agent: Context.Primary.Agent.t) =>
        addToChannelLink(agent.id, channelId),
      flex: 1,
    };

    let all: list(column) = [|agentColumn, inviteColumn|];

    let title =
      <FC.Base.Div float=`left>
        <FC.PageCard.HeaderRow.Title>
          {"Invite Agents" |> ReasonReact.string}
        </FC.PageCard.HeaderRow.Title>
      </FC.Base.Div>;

    let table =
      Foretold__GraphQL.Queries.Agents.componentUsers(
        ~excludeChannelId=channelId, agents =>
        agents
        |> E.HttpResponse.fmap(agents => {
             let dataSource =
               agents
               |> Js.Array.filter((agent: Context.Primary.Agent.t) =>
                    switch (agent.name) {
                    | Some(name) when name != "" => true
                    | _ => false
                    }
                  );

             Table.fromColumns(all, dataSource);
           })
        |> E.HttpResponse.withReactDefaults
      );

    SLayout.LayoutConfig.make(
      ~head=<div> title </div>,
      ~body=<FC.PageCard.Body> table </FC.PageCard.Body>,
    )
    |> layout;
  },
};