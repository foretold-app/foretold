open Rationale.Function.Infix;
open Utils;
open Measurable__Index__Types;

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

module LoadedAndSelected = {
  open Measurable__Index__Types.LoadedAndSelected;

  let component = (t: t, send) =>
    <>
      <SLayout.Header>
        {
          SLayout.channelBack(
            ~channel=t.loadedResources.channel,
            ~onClick=_ => send(Actions.deselect),
            (),
          )
        }
        {
          itemHeader(
            t.loadedResources.channel,
            () => send(Actions.selectIncrement),
            () => send(Actions.selectDecrement),
            Actions.canDecrement(t),
            Actions.canIncrement(t),
          )
        }
      </SLayout.Header>
      <SLayout.MainSection>
        <C.Measurable.FullPresentation
          id={t.selectedMeasurable.id}
          loggedInUser={t.loggedInUser}
        />
      </SLayout.MainSection>
    </>;
};

module LoadedAndUnselected = {
  open Measurable__Index__Types.LoadedAndUnselected;

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

  let component = (t: t, send) => {
    let channel = t.loadedResources.channel;
    let measurables = t.loadedResources.measurables;
    let loggedInUser = t.loggedInUser;
    <>
      {
        itemHeader(
          channel,
          () => send(Actions.selectNextPage),
          () => send(Actions.selectLastPage),
          Actions.canDecrement(t),
          Actions.canIncrement(t),
        )
      }
      <SLayout.MainSection>
        {
          E.React.showIf(
            shouldShowSeriesCollection(t),
            <>
              {"Series List" |> ste |> E.React.inH2}
              <div className=SeriesItems.items>
                {
                  filteredSeriesCollection(t)
                  |> Array.map((x: SeriesCollectionQuery.series) =>
                       <div
                         className=SeriesItems.item
                         onClick={
                           _e =>
                             Context.Routing.Url.push(
                               SeriesShow(channel.id, x.id),
                             )
                         }>
                         <C.Series.Card series=x />
                       </div>
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
};

module MeasurableIndexDataState = {
  open Measurable__Index__Types.MeasurableIndexDataState;
  let toComponent = (send, state: state) =>
    switch (state) {
    | InvalidIndexError => "Item Not Valid" |> ste
    | Unloaded(_) => "Loading" |> ste
    | LoadedAndUnselected(loadedAndUnselected) =>
      LoadedAndUnselected.component(loadedAndUnselected, send)
    | LoadedAndSelected(loadedAndSelected) =>
      LoadedAndSelected.component(loadedAndSelected, send)
    };
};

module Components = {};