open Utils;
open E;
open Css;
open SLayout;
open Foretold__GraphQL;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("Redirecting...");

let row =
  style([
    width(`percent(100.0)),
    borderBottom(`px(1), `solid, hex("eee")),
    display(`flex),
    flexDirection(`row),
    paddingLeft(`px(8)),
    paddingRight(`px(8)),
    paddingTop(`px(8)),
    paddingBottom(`px(7)),
    cursor(`pointer),
    backgroundColor(`hex("fff")),
    selector(":last-child", [borderBottom(`px(0), `solid, hex("eee"))]),
    selector(":hover", [backgroundColor(`hex("eef0f3"))]),
  ]);

let column = style([flex(1)]);

let table =
  style([
    overflowY(`auto),
    borderRadius(`px(2)),
    border(`px(1), `solid, `hex("ddd")),
    marginTop(`em(2.)),
    marginBottom(`em(2.)),
  ]);

let nameS = style([fontWeight(`black), fontSize(`em(1.2))]);
let name = "#general";
let description = "A channel for doing X and Y";
let userCount = "8";
let make = (~loggedInUser: Context.Primary.User.t, _children) => {
  ...component,
  render: _ =>
    Queries.Channels.component(channels => {
      let row = (channel: Context.Primary.Channel.t) =>
        <div className=row>
          <div className=column>
            <div className=nameS> {channel.name |> ste} </div>
            {
              channel.description
              |> E.O.fmap(ste)
              |> E.O.fmap(E.React.inP)
              |> E.O.React.defaultNull
            }
          </div>
          <div className=column>
            {
              loggedInUser
              |> (r => r.agent)
              |> E.O.fmap((agent: Context.Primary.Agent.t) =>
                   Foretold__GraphQL.Mutations.ChannelJoin.Mutation.make(
                     (mutation, _) =>
                     <Antd.Button
                       _type=`primary
                       onClick={
                         _ =>
                           Foretold__GraphQL.Mutations.ChannelJoin.mutate(
                             mutation,
                             agent.id,
                             channel.id,
                           )
                       }>
                       {"Join" |> ste}
                     </Antd.Button>
                   )
                   |> E.React.el
                 )
              |> E.O.React.defaultNull
            }
          </div>
        </div>;
      <>
        <SLayout.Header>
          {SLayout.Header.textDiv("Channels")}
        </SLayout.Header>
        <SLayout.MainSection>
          <div className=table>
            {channels |> E.A.fmap(row) |> ReasonReact.array}
          </div>
        </SLayout.MainSection>
      </>;
    }),
};