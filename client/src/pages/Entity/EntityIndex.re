open Utils;
open Rationale.Function.Infix;

let component = ReasonReact.statelessComponent("EntityShow");

let columns = [|
  Antd.Table.TableProps.make_column(
    ~title="Agent",
    ~dataIndex="agent",
    ~key="agent",
    ~width=2,
    ~render=
      (~text, ~record, ~index) =>
        <a
          onClick={_ => Context.Routing.Url.push(EntityShow(record##id))}
          href="">
          {record##name |> ste}
        </a>,
    (),
  ),
  Antd.Table.TableProps.make_column(
    ~title="Remove",
    ~dataIndex="role",
    ~key="actions2",
    ~width=2,
    ~render=(~text, ~record, ~index) => record##id |> ste,
    (),
  ),
|];

let dataSource =
  EKen.Things.getAll
  |> EKen.Things.withNames
  |> E.A.fmap((r: Graph_T.T.thing) =>
       {
         "key": r |> Graph_T.Thing.id,
         "id": r |> Graph_T.Thing.id,
         "name": r |> EKen.Thing.getName,
       }
     );

let make = (~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ =>
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("All Entities"),
      ~body=<Antd.Table columns dataSource size=`small />,
    )
    |> layout,
};