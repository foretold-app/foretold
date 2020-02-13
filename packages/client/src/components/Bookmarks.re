module Styles = {
  let iconOuter = Css.(style([marginTop(`em(-0.25))]));
  let icon = Css.(style([color(`hex("2a456c")), marginRight(`em(0.5))]));
  let counter = Css.(style([marginLeft(`em(0.5))]));
};

let text = isToggled =>
  switch (isToggled) {
  | true => "Bookmarked"
  | _ => "Bookmark"
  };

module Button = {
  [@react.component]
  let make = (~isToggled, ~counter, ~onClick) => {
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
          <Div flex={`num(1.)}> {text(isToggled) |> Utils.ste} </Div>
          {<Div flex={`num(1.)} className=Styles.counter>
             {counter |> string_of_int |> Utils.ste}
           </Div>
           |> Client.E.React2.showIf(counter > 0)}
        </Div>
      </Button>
    );
  };
};

module BookmarkChannelButton = {
  [@react.component]
  let make = (~mutation, ~channel: Types.channel) => {
    let ((isToggled, counter), setIsToggled) =
      React.useState(_ => {
        let initialBookmarkCount = channel.bookmarksCount |> E.O.default(0);
        let initialIsBookmarked = channel.isBookmarked |> E.O.toBool2;
        (initialIsBookmarked, initialBookmarkCount);
      });

    let onClick = _ => {
      setIsToggled(_ => {
        ChannelBookmarkToogle.mutate(mutation, channel.id);
        isToggled ? (false, counter - 1) : (true, counter + 1);
      });
    };

    <Button onClick isToggled counter />;
  };
};

module BookmarkNotebookButton = {
  [@react.component]
  let make = (~mutation, ~notebook: Types.notebook) => {
    let ((isToggled, counter), setIsToggled) =
      React.useState(_ => {
        let initialBookmarkCount = notebook.bookmarksCount |> E.O.default(0);
        let initialIsBookmarked = notebook.isBookmarked |> E.O.toBool2;
        (initialIsBookmarked, initialBookmarkCount);
      });

    let onClick = _ => {
      setIsToggled(_ => {
        NotebookBookmarkToogle.mutate(mutation, notebook.id);
        isToggled ? (false, counter - 1) : (true, counter + 1);
      });
    };

    <Button onClick isToggled counter />;
  };
};

module BookmarkChannel = {
  [@react.component]
  let make = (~channel) =>
    <ChannelBookmarkToogle.Mutation>
      ...{(mutation, _) => <BookmarkChannelButton mutation channel />}
    </ChannelBookmarkToogle.Mutation>;
};

module BookmarkNotebook = {
  [@react.component]
  let make = (~notebook) =>
    <NotebookBookmarkToogle.Mutation>
      ...{(mutation, _) => <BookmarkNotebookButton mutation notebook />}
    </NotebookBookmarkToogle.Mutation>;
};