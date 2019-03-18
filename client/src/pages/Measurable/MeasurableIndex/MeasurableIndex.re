open Utils;
open Rationale;
open Result.Infix;

module SeriesItems = {
  open Css;
  let items =
    style([
      display(`flex),
      flexWrap(`wrap),
      marginTop(`em(1.0)),
      marginBottom(`em(1.0)),
    ]);
  let item =
    style([
      Css.float(`left),
      margin4(
        ~top=`em(0.0),
        ~left=`em(0.0),
        ~right=`em(0.5),
        ~bottom=`em(0.5),
      ),
    ]);
};

type state = {
  page: int,
  selected: option(int),
};
type action =
  | NextPage
  | LastPage
  | Select(option(int))
  | SelectIncrement
  | SelectDecrement;

let component = ReasonReact.reducerComponent("MeasurableIndex");

let itemsPerPage = 20;

let itemHeader = (channel: string, onForward, onBackward, isAtStart, isAtEnd) =>
  <>
    {SLayout.channelLink(channel)}
    <Antd.Button onClick={_ => onBackward()} disabled=isAtStart>
      <Icon.Icon icon="ARROW_LEFT" />
    </Antd.Button>
    <Antd.Button onClick={_ => onForward()} disabled=isAtEnd>
      <Icon.Icon icon="ARROW_RIGHT" />
    </Antd.Button>
    {SLayout.button(channel)}
  </>;

let selectedView =
    (
      ~channel: string,
      ~loggedInUser: GetUser.t,
      ~send,
      ~measurables: array(DataModel.Measurable.t),
      ~index: int,
    ) => {
  let measurable = E.A.get(measurables, index);
  let itemsOnPage = measurables |> Array.length;
  <>
    <SLayout.Header>
      {
        SLayout.channelBack(
          ~channelName=channel,
          ~onClick=_ => send(Select(None)),
          (),
        )
      }
      {
        itemHeader(
          channel,
          () => send(SelectIncrement),
          () => send(SelectDecrement),
          index == 0,
          index == itemsOnPage - 1,
        )
      }
    </SLayout.Header>
    <SLayout.MainSection>
      {
        switch (measurable) {
        | Some(m) => <MeasurableShow__Component id={m.id} loggedInUser />
        | None => "Item not found" |> ste
        }
      }
    </SLayout.MainSection>
  </>;
};

let deselectedView =
    (
      ~channel: string,
      ~loggedInUser: GetUser.t,
      ~send,
      ~state,
      ~measurables: array(DataModel.Measurable.t),
      ~seriesCollection: array(GetSeriesCollection.series),
    ) => {
  let seriesList =
    seriesCollection
    |> E.A.filter((x: GetSeriesCollection.series) =>
         x.channel == channel && x.measurableCount !== Some(0)
       );
  <>
    {
      itemHeader(
        channel,
        () => send(NextPage),
        () => send(LastPage),
        state.page == 0,
        measurables->E.A.length < itemsPerPage,
      )
      ->E.U.toA
      ->SLayout.Header.make
      |> E.React.el
    }
    <SLayout.MainSection>
      {
        E.React.showIf(
          state.page == 0 && seriesList |> E.A.length > 0,
          <>
            <h2> {"Series List" |> ste} </h2>
            <div className=SeriesItems.items>
              {
                seriesList
                |> Array.map((x: GetSeriesCollection.series) =>
                     <div
                       className=SeriesItems.item
                       onClick={
                         _e => Urls.pushToLink(SeriesShow(x.channel, x.id))
                       }>
                       <SeriesCard series=x />
                     </div>
                   )
                |> ReasonReact.array
              }
            </div>
          </>,
        )
      }
      <MeasurableIndex__Table
        measurables
        loggedInUser
        showExtraData=true
        onSelect={
          e =>
            send(
              Select(
                measurables
                |> E.A.findIndex((r: DataModel.Measurable.t) => r.id == e.id),
              ),
            )
        }
      />
    </SLayout.MainSection>
  </>;
};

let make = (~channel: string, ~loggedInUser: GetUser.t, _children) => {
  ...component,
  initialState: () => {page: 0, selected: None},
  reducer: (action, state) =>
    switch (action) {
    | NextPage => ReasonReact.Update({...state, page: state.page + 1})
    | LastPage => ReasonReact.Update({...state, page: state.page - 1})
    | Select((num: option(int))) =>
      ReasonReact.Update({...state, selected: num})
    | SelectIncrement =>
      ReasonReact.Update({
        ...state,
        selected: state.selected |> E.O.fmap(r => r + 1),
      })
    | SelectDecrement =>
      ReasonReact.Update({
        ...state,
        selected: state.selected |> E.O.fmap(r => r - 1),
      })
    },
  render: ({state, send}) =>
    GetSeriesCollection.component(
      (seriesCollection: array(GetSeriesCollection.series)) =>
      GetMeasurables.component(
        channel,
        state.page,
        itemsPerPage,
        (measurables: array(DataModel.Measurable.t)) =>
        switch (state.selected) {
        | Some(index) =>
          selectedView(~channel, ~loggedInUser, ~send, ~measurables, ~index)
        | _ =>
          deselectedView(
            ~channel,
            ~loggedInUser,
            ~send,
            ~state,
            ~measurables,
            ~seriesCollection,
          )
        }
      )
    ),
};