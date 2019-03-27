open Utils;
open Foretold__GraphQL;

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
  selectedIndex: option(int),
};
type action =
  | NextPage
  | LastPage
  | Select(option(int))
  | SelectIncrement
  | SelectDecrement;

let component = ReasonReact.reducerComponent("MeasurableIndex");

let itemsPerPage = 20;

let itemHeader =
    (
      channel: Context.Primary.Channel.t,
      onForward,
      onBackward,
      isAtStart,
      isAtEnd,
    ) =>
  <>
    {SLayout.channelink(channel)}
    <Antd.Button onClick={_ => onBackward()} disabled=isAtStart>
      <Icon.Icon icon="ARROW_LEFT" />
    </Antd.Button>
    <Antd.Button onClick={_ => onForward()} disabled=isAtEnd>
      <Icon.Icon icon="ARROW_RIGHT" />
    </Antd.Button>
    {C.Channel.SimpleHeader.button(channel.id)}
  </>;

let selectedView =
    (
      ~channel: Context.Primary.Channel.t,
      ~loggedInUser: Context.Primary.User.t,
      ~send,
      ~measurables: array(Context.Primary.Measurable.t),
      ~index: int,
    ) => {
  let measurable = E.A.get(measurables, index);
  let itemsOnPage = measurables |> Array.length;
  <>
    <SLayout.Header>
      {SLayout.channelBack(~channel, ~onClick=_ => send(Select(None)), ())}
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
        | Some(m) => <C.Measurable.FullPresentation id={m.id} loggedInUser />
        | None => "Item not found" |> ste
        }
      }
    </SLayout.MainSection>
  </>;
};

let deselectedView =
    (
      ~channel: Context.Primary.Channel.t,
      ~loggedInUser: Context.Primary.User.t,
      ~send,
      ~state,
      ~measurables: array(Context.Primary.Measurable.t),
      ~seriesCollection: array(Queries.SeriesCollection.series),
    ) => {
  let seriesList =
    seriesCollection
    |> E.A.filter((x: Queries.SeriesCollection.series) =>
         x.measurableCount !== Some(0)
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
            {"Series List" |> ste |> E.React.inH2}
            <div className=SeriesItems.items>
              {
                seriesList
                |> Array.map((x: Queries.SeriesCollection.series) =>
                     switch (x.channel) {
                     | Some(channel) =>
                       <div
                         className=SeriesItems.item
                         onClick=(
                           _e =>
                             Context.Routing.Url.push(
                               SeriesShow(channel.id, x.id),
                             )
                         )>
                         <C.Series.Card series=x />
                       </div>
                     | None => E.React.null
                     }
                   )
                |> ReasonReact.array
              }
            </div>
          </>,
        )
      }
      <C.Measurables.BasicTable
        measurables
        loggedInUser
        showExtraData=true
        onSelect={
          e =>
            send(
              Select(
                measurables
                |> E.A.findIndex((r: Context.Primary.Measurable.t) =>
                     r.id == e.id
                   ),
              ),
            )
        }
      />
    </SLayout.MainSection>
  </>;
};

/* let foo = fn => (r => 38) |> ((e: int => int) => e(3)); */

let combine2QueryResults = (query1, query2, fnlast) =>
  query1(response1 => query2(response2 => fnlast(response1, response2)));

let top2Queries = (channelId, fn) =>
  combine2QueryResults(
    Queries.Channel.component2(~id=channelId),
    Queries.SeriesCollection.component2,
    (
      channelResponse: Queries.Channel.ttt,
      seriesResponse: Queries.SeriesCollection.ttt,
    ) =>
    fn(channelResponse, seriesResponse)
  );

let make =
    (~channelId: string, ~loggedInUser: Context.Primary.User.t, _children) => {
  ...component,
  initialState: () => {page: 0, selectedIndex: None},
  reducer: (action, state) =>
    switch (action) {
    | NextPage =>
      ReasonReact.Update({selectedIndex: None, page: state.page + 1})
    | LastPage =>
      ReasonReact.Update({selectedIndex: None, page: state.page - 1})
    | Select((num: option(int))) =>
      ReasonReact.Update({...state, selectedIndex: num})
    | SelectIncrement =>
      ReasonReact.Update({
        ...state,
        selectedIndex: state.selectedIndex |> E.O.fmap(E.I.increment),
      })
    | SelectDecrement =>
      ReasonReact.Update({
        ...state,
        selectedIndex: state.selectedIndex |> E.O.fmap(E.I.decrement),
      })
    },
  render: ({state, send}) =>
    top2Queries(channelId, (channelResponse, seriesResponse) =>
      switch (channelResponse, seriesResponse) {
      | (Success(channel), Success(series)) =>
        Queries.Measurables.component2(
          channelId,
          state.page,
          itemsPerPage,
          (
            measurablesResponse:
              Client.E.HtppResponse.t(array(Context.Primary.Measurable.t)),
          ) =>
          switch (measurablesResponse) {
          | Success(measurables) =>
            switch (state.selectedIndex) {
            | Some(index) =>
              selectedView(
                ~channel,
                ~loggedInUser,
                ~send,
                ~measurables,
                ~index,
              )
            | _ =>
              deselectedView(
                ~channel,
                ~loggedInUser,
                ~send,
                ~state,
                ~measurables,
                ~seriesCollection=series,
              )
            }
          | _ => <div />
          }
        )
      | _ => <div />
      }
    ),
};