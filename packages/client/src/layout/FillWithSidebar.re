[@bs.config {jsx: 3}];

module Styles = {
  open Css;
  let outer =
    style([
      display(`flex),
      flexDirection(`row),
      minHeight(`percent(100.0)),
      backgroundColor(`hex("F0F2F5")),
    ]);
  let left =
    style([
      flex(`num(1.)),
      backgroundColor(`hex("283c5b")),
      minWidth(`px(200)),
    ]);
  let right =
    style([
      flex(`num(7.)),
      width(`percent(100.0)),
      display(`flex),
      minHeight(`vh(100.0)),
      flexDirection(`column),
    ]);
  let rightInner = style([flex(`num(1.))]);
};

[@react.component]
let make = (~channelId=None, ~children=ReasonReact.null) => {
  <Providers.AppContext.Consumer>
    ...{({loggedUser}) =>
      <div className=Styles.outer>
        {loggedUser
         |> E.O.React.fmapOrNull(_ =>
              <div className=Styles.left>
                <Sidebar channelId loggedUser />
              </div>
            )}
        <div className=Styles.right>
          <div className=Styles.rightInner>
            <Header loggedUser />
            <div> children </div>
          </div>
          <Footer />
        </div>
      </div>
    }
  </Providers.AppContext.Consumer>;
};
