let component = ReasonReact.statelessComponent("ChannelInvite");

type column = Table.column(Primary.Agent.t);

let make =
    (
      ~channelId: string,
      ~loggedInUser: Primary.User.t,
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

    let agentColumn: column =
      Table.Column.make(
        ~name="Agent" |> ReasonReact.string,
        ~render=
          (agent: Primary.Agent.t) =>
            <Foretold__Components__Link
              linkType={
                Internal(
                  Agent({agentId: agent.id, subPage: AgentMeasurements}),
                )
              }>
              {agent.name
               |> Rationale.Option.default("")
               |> ReasonReact.string}
            </Foretold__Components__Link>,
        (),
      );

    let inviteColumn: column =
      Table.Column.make(
        ~name="Invite" |> ReasonReact.string,
        ~render=
          (agent: Primary.Agent.t) => addToChannelLink(agent.id, channelId),
        (),
      );

    let all: array(column) = [|agentColumn, inviteColumn|];

    let title =
      <FC.Base.Div float=`left>
        <FC.PageCard.HeaderRow.Title>
          {"Invite Agents" |> ReasonReact.string}
        </FC.PageCard.HeaderRow.Title>
      </FC.Base.Div>;

    let onSuccess = agents => {
      let dataSource =
        agents
        |> Js.Array.filter((agent: Primary.Agent.t) =>
             switch (agent.name) {
             | Some(name) when name != "" => true
             | _ => false
             }
           );

      Table.fromColumns(all, dataSource, ());
    };

    let onError = e => <SLayout.Error e />;

    let loadingFn = () => <SLayout.Spin />;

    let table =
      Foretold__GraphQL.Queries.Agents.componentUsers(
        ~excludeChannelId=channelId, agents =>
        agents |> E.HttpResponse.flatten(onSuccess, onError, loadingFn)
      );

    SLayout.LayoutConfig.make(
      ~head=<div> title </div>,
      ~body=<FC.PageCard.Body> table </FC.PageCard.Body>,
    )
    |> layout;
  },
};