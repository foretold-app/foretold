open Utils;
open E;
open Css;
open SLayout;
open Foretold__GraphQL;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("ChannelMembers");

let make =
    (
      ~channelId: string,
      ~loggedInUser: Context.Primary.User.t,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ => {
    let page =
      Queries.ChannelMemberships.component(~id=channelId, memberships =>
        memberships
        |> E.HttpResponse.fmap(memberships =>
             <div>
               {
                 memberships
                 |> E.A.fmap((r: Context.Primary.Types.channelMembership) =>
                      r.agent
                      |> E.O.bind(_, r => r.name)
                      |> E.O.default("")
                      |> ste
                    )
                 |> ReasonReact.array
               }
             </div>
           )
        |> E.HttpResponse.withReactDefaults
      );

    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Channel Members"),
      ~body=page,
    )
    |> layout;
  },
};