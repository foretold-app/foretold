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
      backgroundColor(hex("eee")),
      padding2(~v=`em(0.2), ~h=`em(0.7)),
      borderRadius(`px(4)),
      margin4(
        ~top=`em(0.0),
        ~left=`em(0.0),
        ~right=`em(0.5),
        ~bottom=`em(0.5),
      ),
      backgroundColor(`hex("ebe9f3")),
      border(`px(1), `solid, `hex("dadae4")),
      color(`hex("0b2d67")),
      cursor(`pointer),
      minWidth(`em(23.)),
      selector(":hover", [backgroundColor(`hex("dad7e8"))]),
      selector("p", [marginBottom(`px(0))]),
    ]);

  let heading =
    style([
      color(`hex("0b2d67")),
      fontWeight(`bold),
      fontSize(`em(1.2)),
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

let indexChannelHeader =
    (channel: option(string), onForward, onBackward, isAtStart, isAtEnd) =>
  channel
  |> E.O.fmap(c =>
       <div>
         {SLayout.channelLink(c)}
         <Antd.Button onClick={_ => onBackward()} disabled=isAtStart>
           <Icon.Icon icon="ARROW_LEFT" />
         </Antd.Button>
         <Antd.Button onClick={_ => onForward()} disabled=isAtEnd>
           <Icon.Icon icon="ARROW_RIGHT" />
         </Antd.Button>
         {SLayout.button(c)}
       </div>
     )
  |> E.O.React.defaultNull;

let itemHeader =
    (
      channel: option(string),
      onForward,
      onBackward,
      onBack,
      isAtStart,
      isAtEnd,
    ) =>
  channel
  |> E.O.fmap(c =>
       <div>
         {SLayout.channelBack(~channelName=c, ~onClick=_ => onBack(), ())}
         {SLayout.channelLink(c)}
         <Antd.Button onClick={_ => onBackward()} disabled=isAtStart>
           <Icon.Icon icon="ARROW_LEFT" />
         </Antd.Button>
         <Antd.Button onClick={_ => onForward()} disabled=isAtEnd>
           <Icon.Icon icon="ARROW_RIGHT" />
         </Antd.Button>
         {SLayout.button(c)}
       </div>
     )
  |> E.O.React.defaultNull;

let selectedView =
    (
      ~channel: string,
      ~loggedInUser: GetUser.t,
      ~send,
      ~measurables: array(DataModel.measurable),
      ~index: int,
    ) => {
  let measurable = E.A.get(measurables, index);
  let itemsOnPage = measurables |> Array.length;
  <div>
    <SLayout.Header>
      {
        itemHeader(
          Some(channel),
          () => send(SelectIncrement),
          () => send(SelectDecrement),
          () => send(Select(None)),
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
  </div>;
};

let deselectedView =
    (
      ~channel: string,
      ~loggedInUser: GetUser.t,
      ~send,
      ~state,
      ~measurables: array(DataModel.measurable),
      ~seriesCollection: array(GetSeriesCollection.series),
    ) => {
  let seriesList =
    seriesCollection
    |> E.A.filter((x: GetSeriesCollection.series) =>
         x.channel == channel && x.measurableCount !== Some(0)
       );
  <div>
    <SLayout.Header>
      {
        indexChannelHeader(
          Some(channel),
          () => send(NextPage),
          () => send(LastPage),
          state.page == 0,
          measurables |> Array.length < itemsPerPage,
        )
      }
    </SLayout.Header>
    <SLayout.MainSection>
      {
        showIf(
          state.page == 0 && seriesList |> E.A.length > 0,
          <div>
            <h2> {"Series List" |> ste} </h2>
            <div className=SeriesItems.items>
              {
                seriesList
                /* TODO: This should only query to get the series, it shouldn't do filtering here. */
                |> Array.map((x: GetSeriesCollection.series) =>
                     <div
                       className=SeriesItems.item
                       onClick={
                         _e => Urls.pushToLink(SeriesShow(x.channel, x.id))
                       }>
                       <span className=SeriesItems.heading>
                         <Icon.Icon icon="LAYERS" />
                         {x.name |> E.O.default("") |> ste}
                       </span>
                       {
                         x.description
                         |> E.O.fmap(d => <p> {d |> ste} </p>)
                         |> E.O.React.defaultNull
                       }
                       {
                         x.measurableCount
                         |> E.O.fmap(d =>
                              <p>
                                {
                                  d
                                  |> string_of_int
                                  |> (e => e ++ " items")
                                  |> ste
                                }
                              </p>
                            )
                         |> E.O.React.defaultNull
                       }
                     </div>
                   )
                |> ReasonReact.array
              }
            </div>
          </div>,
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
                |> Array.to_list
                |> Rationale.RList.findIndex((r: DataModel.measurable) =>
                     r.id == e.id
                   ),
              ),
            )
        }
      />
    </SLayout.MainSection>
  </div>;
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
        (measurables: array(DataModel.measurable)) =>
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