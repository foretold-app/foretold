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
  let toHeader = (send, state: state) =>
    switch (state) {
    | InvalidIndexError(channel) => SLayout.channelink(channel)
    | WithChannelButNotQuery(c) => SLayout.channelink(c.channel)
    | LoadedAndUnselected(loadedAndUnselected) =>
      LoadedAndUnselected.header(loadedAndUnselected, send)
    | LoadedAndSelected(loadedAndSelected) =>
      LoadedAndSelected.header(loadedAndSelected, send)
    | WithoutChannel(channel) => <div />
    };

  let toBody = (send, state: state) =>
    switch (state) {
    | InvalidIndexError(e) => "Item Not Valid" |> ste
    | WithChannelButNotQuery(_) => "Loading..." |> ste
    | LoadedAndUnselected(loadedAndUnselected) =>
      LoadedAndUnselected.body(loadedAndUnselected, send)
    | LoadedAndSelected(loadedAndSelected) =>
      LoadedAndSelected.body(loadedAndSelected)
    | WithoutChannel(channel) => "Loading..." |> ste
    };

  let toComponent = (send, state: state) =>
    <>
      <SLayout.Header> {toHeader(send, state)} </SLayout.Header>
      <SLayout.MainSection> {toBody(send, state)} </SLayout.MainSection>
    </>;
};

module Components = {};