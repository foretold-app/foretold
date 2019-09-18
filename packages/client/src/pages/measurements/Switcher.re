let head = (~channelId, ~paginationPage, ~subTab, ()) =>
  <PaginationX paginationPage subTab />;

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
    let head = head(~subTab);
    switch (subTab) {
    | ByMeasurements => <Measurements measurableId loggedInUser />
    | ByScores =>
      <LeaderboardMeasurables measurableId={Some(measurableId)} head />
    };
  },
};