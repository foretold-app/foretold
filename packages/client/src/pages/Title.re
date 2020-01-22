[@react.component]
let make = (~route: Routing.Route.t) => {
  switch (route) {
  | Channel({channelId, subPage: Measurables(_)}) =>
    ChannelGet.component2(~id=channelId, result =>
      switch (result) {
      | Success(channel) =>
        let title = [|channel.name, Lang.Title.main|] |> E.Title.toString;
        <ForetoldComponents.Title title />;
      | _ => <Null />
      }
    )
  | ChannelIndex =>
    <ForetoldComponents.Title
      title={[|Lang.Title.communities, Lang.Title.main|] |> E.Title.toString}
    />
  | Login =>
    <ForetoldComponents.Title
      title={[|Lang.Title.login, Lang.Title.main|] |> E.Title.toString}
    />
  | ChannelNew =>
    <ForetoldComponents.Title
      title={[|Lang.Title.channelNew, Lang.Title.main|] |> E.Title.toString}
    />
  | Terms =>
    <ForetoldComponents.Title
      title={[|Lang.Title.terms, Lang.Title.main|] |> E.Title.toString}
    />
  | _ =>
    <ForetoldComponents.Title
      title={[|Lang.Title.main|] |> E.Title.toString}
    />
  };
};