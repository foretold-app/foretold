module Styles = {
  open Css;
  let fullPage =
    style([
      display(`flex),
      flexDirection(`column),
      minHeight(`percent(100.0)),
      minHeight(`vh(100.0)),
      width(`percent(100.0)),
      backgroundColor(`hex("F0F2F5")),
      flex(`num(7.)),
    ]);
  let left =
    style([
      flex(`num(1.)),
      backgroundColor(`hex("fff")),
      minWidth(`px(200)),
    ]);
  let aboveFooter = style([flex(`num(1.))]);
};

[@react.component]
let make = (~channelId=None, ~children=<Null />) => {
  let context = React.useContext(Providers.app);
  <div className=Styles.fullPage>
    <div className=Styles.aboveFooter>
      <Header loggedUser={context.loggedUser} />
      children
    </div>
    <Footer />
  </div>;
};