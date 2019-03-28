open Rationale.Function.Infix;
open Utils;
open Measurable__Index__Types;

let itemsPerPage = 20;

let itemHeader =
    (
      channel: Context.Primary.Channel.t,
      onForward,
      onBackward,
      canDecrement,
      canIncrement,
    ) =>
  <>
    {SLayout.channelink(channel)}
    <Antd.Button onClick={_ => onBackward()} disabled={!canDecrement}>
      <Icon.Icon icon="ARROW_LEFT" />
    </Antd.Button>
    <Antd.Button onClick={_ => onForward()} disabled={!canIncrement}>
      <Icon.Icon icon="ARROW_RIGHT" />
    </Antd.Button>
    {C.Channel.SimpleHeader.button(channel.id)}
  </>;

module LoadedAndSelected = {
  open Measurable__Index__Types.LoadedAndSelected;

  let header = (t: t, send) =>
    <>
      {
        SLayout.channelBack(
          ~channel=t.channel,
          ~onClick=_ => send(Actions.deselect),
          (),
        )
      }
      {
        itemHeader(
          t.channel,
          () => send(Actions.selectIncrement),
          () => send(Actions.selectDecrement),
          Actions.canDecrement(t),
          Actions.canIncrement(t),
        )
      }
    </>;

  let body = (t: t) =>
    <C.Measurable.FullPresentation
      id={t.selectedMeasurable.id}
      loggedInUser={t.loggedInUser}
    />;
};

module LoadedAndUnselected = {
  open Measurable__Index__Types.LoadedAndUnselected;

  let header = (t: t, send) => {
    let channel = t.channel;
    itemHeader(
      channel,
      () => send(Actions.selectNextPage),
      () => send(Actions.selectLastPage),
      Actions.canDecrement(t),
      Actions.canIncrement(t),
    );
  };

  let seriesList = (t: t) =>
    <>
      {"Series List" |> ste |> E.React.inH2}
      {
        C.SeriesCollection.SeriesCards.make(
          t.channel.id,
          filteredSeriesCollection(t),
        )
      }
    </>;

  let body = (t: t, send) => {
    let measurables = t.loadedResources.measurables;
    let loggedInUser = t.loggedInUser;
    <>
      {E.React.showIf(shouldShowSeriesCollection(t), seriesList(t))}
      <C.Measurables.BasicTable
        measurables
        loggedInUser
        showExtraData=true
        onSelect={e => send(selectMeasurableOfMeasurableId(t, e.id))}
      />
    </>;
  };
};

module MeasurableIndexDataState = {
  open Measurable__Index__Types.MeasurableIndexDataState;

  let toLayoutInput = (send, state: state) => {
    let lmake = SLayout.LayoutConfig.make;
    switch (state) {
    | InvalidIndexError(channel) =>
      lmake(~head=SLayout.channelink(channel), ~body="Item Not Valid" |> ste)
    | WithChannelButNotQuery(c) =>
      lmake(~head=SLayout.channelink(c.channel), ~body="Loading..." |> ste)
    | LoadedAndUnselected(l) =>
      lmake(
        ~head=LoadedAndUnselected.header(l, send),
        ~body=LoadedAndUnselected.body(l, send),
      )
    | LoadedAndSelected(l) =>
      lmake(
        ~head=LoadedAndSelected.header(l, send),
        ~body=LoadedAndSelected.body(l),
      )
    | WithoutChannel(channelResponse) =>
      lmake(~head=<div />, ~body="Loading..." |> ste)
    };
  };
};