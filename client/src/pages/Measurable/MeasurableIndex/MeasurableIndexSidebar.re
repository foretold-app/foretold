open Utils;
open Rationale;
open Queries;

module Styles = {
  open Css;
  let over = style([display(`flex), flexDirection(`column)]);
  let hash =
    style([marginRight(`px(5)), color(`rgba((255, 255, 255, 0.3)))]);
  let item =
    style([
      flex(1),
      color(`rgba((255, 255, 255, 0.6))),
      hover([color(`rgba((255, 255, 255, 0.6)))]),
      fontSize(`em(1.2)),
      padding4(~top=`px(4), ~bottom=`px(4), ~left=`px(14), ~right=`px(2)),
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
let make = (~channel, children) => {
  ...component,
  render: _self =>
    <UseRouterForLinks>
      <div className=Styles.over>
        {
          ["general", "random", "foretold", "ozziegooen", "lesswrong"]
          |> List.map(e =>
               <a
                 href={"/c/" ++ e}
                 className={e == channel ? Styles.selectedItem : Styles.item}>
                 <span>
                   <span className=Styles.hash> {"#" |> ste} </span>
                   <span> {e |> ste} </span>
                 </span>
               </a>
             )
          |> Array.of_list
          |> ReasonReact.array
        }
      </div>
    </UseRouterForLinks>,
};