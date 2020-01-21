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
let make = (~channelId=None, ~children=<Null />) => {
  let context = React.useContext(Providers.app);
  <div className=Styles.outer>
    {context.loggedUser
     |> E.O.React.fmapOrNull(_ =>
          <div className=Styles.left>
            <Sidebar channelId loggedUser={context.loggedUser} />
          </div>
        )}
    <div className=Styles.right>
      <div className=Styles.rightInner>
        <Header loggedUser={context.loggedUser} />
        <div> children </div>
      </div>
      <Footer />
    </div>
  </div>;
};