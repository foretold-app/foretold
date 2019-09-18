let head = (~channelId, ~paginationPage, ()) =>
  <PaginationX paginationPage />;

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
    switch (subTab) {
    | ByMeasurements => <Measurements measurableId loggedInUser />
    | ByScores =>
      <LeaderboardMeasurables measurableId={Some(measurableId)} head />
    };
  },
};