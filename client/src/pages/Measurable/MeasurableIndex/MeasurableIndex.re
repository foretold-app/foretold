open Utils;
open Rationale;
open Result.Infix;

type state = {page: int};
type action =
  | NextPage
  | LastPage;

let component = ReasonReact.reducerComponent("Measurables");

let itemsPerPage = 20;

let make = (~channel: string, ~loggedInUser: GetUser.t, _children) => {
  ...component,
  initialState: () => {page: 0},
  reducer: (action, state) =>
    switch (action) {
    | NextPage => ReasonReact.Update({page: state.page + 1})
    | LastPage => ReasonReact.Update({page: state.page - 1})
    },
  render: ({state, send}) =>
    (
      measurables =>
        <div>
          {
            SLayout.indexChannelHeader(
              Some(channel),
              state.page,
              () => send(NextPage),
              () => send(LastPage),
              measurables |> Array.length < itemsPerPage,
            )
          }
          <div className=SLayout.Styles.mainSection>
            <MeasurableIndex__Table
              measurables
              loggedInUser
              showExtraData=true
            />
          </div>
        </div>
    )
    |> GetMeasurables.component(channel, state.page, itemsPerPage),
};