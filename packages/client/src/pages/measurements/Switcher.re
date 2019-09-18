let head = (~channelId, ~measurableId, ~paginationPage, ~subTab, ()) =>
  <PaginationX paginationPage subTab measurableId channelId />;

let component = ReasonReact.statelessComponent("Switcher");
let make =
    (
      ~measurableId: string,
      ~loggedInUser: Types.user,
      ~subTab: Routing.ChannelPage.measurementsTab=ByMeasurements,
      _children,
    ) => {
  ...component,
  render: _ => {
    let head = head(~subTab, ~measurableId);
    switch (subTab) {
    | ByMeasurements => <Measurements measurableId loggedInUser />
    | ByScores =>
      <LeaderboardMeasurables measurableId={Some(measurableId)} head />
    };
  },
};