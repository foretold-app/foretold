type tab = PaginationX.tab;

type state = {tab};

type action =
  | SwitchTab(tab);

let head =
    (~switchTab, ~tab, ~channelId: option(string), ~paginationPage, ()) =>
  <PaginationX switchTab paginationPage tab />;

let component = ReasonReact.reducerComponent("Switcher");
let make =
    (
      ~measurableId: string,
      ~channelId: option(string),
      ~loggedInUser: Types.user,
      _children,
    ) => {
  ...component,
  reducer: (action, _state) =>
    switch (action) {
    | SwitchTab(tab) => ReasonReact.Update({tab: tab})
    },

  initialState: () => {tab: Measurements},
  render: self => {
    let tab = self.state.tab;
    let switchTab = tabToSwitch => self.send(SwitchTab(tabToSwitch));
    let head = head(~switchTab, ~tab);

    switch (tab) {
    | Measurements => <Measurements measurableId loggedInUser head />
    | Scores =>
      <LeaderboardMeasurables
        channelId
        measurableId={Some(measurableId)}
        head
      />
    };
  },
};