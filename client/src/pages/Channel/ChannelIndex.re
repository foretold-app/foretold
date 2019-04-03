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
let make =
    (
      ~loggedInUser: Context.Primary.User.t,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ =>
    Queries.Channels.component(channels => {
      let columns = [|
        Antd.Table.TableProps.make_column(
          ~title="Channel",
          ~dataIndex="id",
          ~key="channelName",
          ~width=1,
          ~render=
            (~text, ~record, ~index) =>
              <span className=nameS> {record##name |> ste} </span>,
          (),
        ),
        Antd.Table.TableProps.make_column(
          ~title="Members",
          ~dataIndex="count",
          ~key="membersCount",
          ~width=1,
          ~render=
            (~text, ~record, ~index) =>
              record##count
              |> E.O.fmap(string_of_int)
              |> E.O.default("")
              |> ste,
          (),
        ),
        Antd.Table.TableProps.make_column(
          ~title="Title",
          ~dataIndex="name",
          ~key="Title",
          ~width=1,
          ~render=
            (~text, ~record, ~index) =>
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
                             record##id,
                           )
                       }>
                       {"Join" |> ste}
                     </Antd.Button>
                   )
                   |> E.React.el
                 )
              |> E.O.React.defaultNull,
          (),
        ),
      |];

      let dataSource =
        channels
        |> E.A.fmap((r: Context.Primary.Channel.t) =>
             {
               "key": r.id,
               "id": r.id,
               "name": r.name,
               "count": r.membershipCount,
             }
           );

      SLayout.LayoutConfig.make(
        ~head=SLayout.Header.textDiv("Channels"),
        ~body=<div> <Antd.Table columns dataSource /> </div>,
      )
      |> layout;
    }),
};