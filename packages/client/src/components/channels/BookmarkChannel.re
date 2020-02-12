module BookmarkChannelButton = {
  module Styles = {
    let iconOuter = Css.(style([marginTop(`em(-0.25))]));
    let icon =
      Css.(style([color(`hex("2a456c")), marginRight(`em(0.5))]));
    let counter = Css.(style([marginLeft(`em(0.5))]));
  };

  [@react.component]
  let make = (~mutation, ~channel: Types.channel) => {
    let ((isToggled, counter), setIsToggled) =
      React.useState(_ => {
        let initialBookmarkCount = channel.bookmarksCount |> E.O.default(0);
        let initialIsBookmarked = channel.isBookmarked |> E.O.toBool2;
        (initialIsBookmarked, initialBookmarkCount);
      });

    let text =
      switch (isToggled) {
      | true => "Bookmarked"
      | _ => "Bookmark"
      };

    let onClick = _ => {
      setIsToggled(_ => {
        ChannelBookmarkToogle.mutate(mutation, channel.id);
        isToggled ? (false, counter - 1) : (true, counter + 1);
      });
    };

    ForetoldComponents.(
      <Button
        variant=Button.Secondary
        size=Button.Medium
        className=GroupHeader.Styles.actionButtonPosition
        onClick>
        <Div
          flexDirection=`row
          justifyContent=`spaceAround
          alignItems=`center
          alignContent=`stretch>
          <Div flex={`num(1.)} className=Styles.iconOuter>
            <ReactKitIcon icon="STAR_FULL" className=Styles.icon />
          </Div>
          <Div flex={`num(1.)}> {text |> Utils.ste} </Div>
          {<Div flex={`num(1.)} className=Styles.counter>
             {counter |> string_of_int |> Utils.ste}
           </Div>
           |> Client.E.React2.showIf(counter > 0)}
        </Div>
      </Button>
    );
  };
};

[@react.component]
let make = (~channel) =>
  <ChannelBookmarkToogle.Mutation>
    ...{(mutation, _) => <BookmarkChannelButton mutation channel />}
  </ChannelBookmarkToogle.Mutation>;