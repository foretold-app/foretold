open Utils;
open Rationale;
open Queries;

module Styles = {
  open Css;
  let over = style([display(`flex), flexDirection(`column)]);
  let hash =
    style([marginRight(`px(5)), color(`rgba((255, 255, 255, 0.3)))]);
  let minorHeader =
    style([
      color(`rgba((255, 255, 255, 0.6))),
      fontSize(`em(1.4)),
      padding4(~top=`px(4), ~bottom=`px(4), ~left=`px(14), ~right=`px(2)),
    ]);
  let sectionPadding = style([height(`em(3.0)), width(`percent(100.0))]);
  let item =
    style([
      flex(1),
      color(`rgba((255, 255, 255, 0.6))),
      hover([color(`rgba((255, 255, 255, 0.6)))]),
      fontSize(`em(1.2)),
      padding4(~top=`px(4), ~bottom=`px(4), ~left=`px(14), ~right=`px(2)),
      cursor(`pointer),
      textDecoration(`none),
      hover([background(`hex("435e90"))]),
      selector("a", [borderBottom(`px(2), `solid, hex("eee"))]),
    ]);
  let selectedItem =
    style([
      flex(1),
      color(`rgba((255, 255, 255, 0.8))),
      hover([color(`rgba((255, 255, 255, 0.8)))]),
      background(`hex("3192ff")),
      fontSize(`em(1.2)),
      cursor(`pointer),
      padding4(~top=`px(4), ~bottom=`px(4), ~left=`px(14), ~right=`px(2)),
      focus([textDecoration(`none)]),
      textDecoration(`none),
    ]);
};

let component = ReasonReact.statelessComponent("MeasurableIndexSidebar");
let make = (~channel, _children) => {
  ...component,
  render: _self => {
    let input: option(Queries.GetUser.Query.t) => ReasonReact.reactElement =
      userQuery =>
        <UseRouterForLinks>
          <div className=Styles.minorHeader> {"User" |> ste} </div>
          {
            switch (userQuery) {
            | Some(query) =>
              open Rationale.Option.Infix;
              let userAgentId = query##user >>= (e => e##agentId);
              let idd =
                switch (userAgentId) {
                | Some(id) => id
                | _ => ""
                };
              ();
              <div>
                <div
                  onClick=(_e => ReasonReact.Router.push("/profile"))
                  className=Styles.item>
                  {"Profile" |> ste}
                </div>
                <div
                  onClick=(
                    _e =>
                      ReasonReact.Router.push(
                        "/agents/" ++ idd ++ "/measurables",
                      )
                  )
                  className=Styles.item>
                  {"Edit Measurables" |> ste}
                </div>
                <div onClick=(_e => Auth0.logout()) className=Styles.item>
                  {"Log Out" |> ste}
                </div>
              </div>;
            | _ =>
              <div onClick=(_e => Auth0.logIn()) className=Styles.item>
                {"Log In" |> ste}
              </div>
            }
          }
          <div className=Styles.over />
          <div className=Styles.sectionPadding />
          <div className=Styles.minorHeader> {"Channels" |> ste} </div>
          <div className=Styles.over>
            {
              ["general", "foretold", "ozziegooen", "lesswrong", "movies"]
              |> List.map(e =>
                   <div
                     onClick={_e => ReasonReact.Router.push("/c/" ++ e)}
                     className={
                       Some(e) == channel ? Styles.selectedItem : Styles.item
                     }>
                     <span>
                       <span className=Styles.hash> {"#" |> ste} </span>
                       <span> {e |> ste} </span>
                     </span>
                   </div>
                 )
              |> Array.of_list
              |> ReasonReact.array
            }
          </div>
        </UseRouterForLinks>;
    SharedQueries.withLoggedInUserQuery(input);
  },
};