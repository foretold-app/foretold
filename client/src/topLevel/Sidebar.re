open Utils;
open Rationale;

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

let component = ReasonReact.statelessComponent("Sidebar");
let make = (~channel, ~loggedInUser: GetUser.t, _children) => {
  ...component,
  render: _self =>
    <div>
      <div className=Styles.minorHeader> {"User" |> ste} </div>
      {
        switch (loggedInUser) {
        | Some(user) =>
          open Rationale.Option.Infix;
          let idd = user.agentId |> E.O.default("");
          <div>
            <div
              onClick=(_e => Urls.pushToLink(Profile)) className=Styles.item>
              {"Profile" |> ste}
            </div>
            <div
              onClick=(_e => Urls.pushToLink(AgentMeasurables(idd)))
              className=Styles.item>
              {"Edit Measurables" |> ste}
            </div>
            <div onClick=(_e => Auth0.logout()) className=Styles.item>
              {"Log Out" |> ste}
            </div>
          </div>;
        | None =>
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
          [
            "general",
            "foretold",
            "ozziegooen",
            "lesswrong",
            "movies",
            "companies",
          ]
          |> E.L.fmap(e =>
               <div
                 onClick={_e => Urls.pushToLink(Channel(e))}
                 className={
                   Some(e) == channel ? Styles.selectedItem : Styles.item
                 }>
                 <span>
                   <span className=Styles.hash> {"#" |> ste} </span>
                   <span> {e |> ste} </span>
                 </span>
               </div>
             )
          |> E.A.of_list
          |> ReasonReact.array
        }
      </div>
    </div>,
};