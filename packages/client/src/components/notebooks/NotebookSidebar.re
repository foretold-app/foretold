module Styles = {
  let topActions = Css.(style([display(`flex), padding(`em(0.2))]));
  let actionButtonPosition =
    Css.(style([float(`right), marginLeft(`em(1.))]));
  let actionButtonsInner = Css.(style([width(`percent(100.0))]));
  let actionButtonsRight = Css.(style([float(`right)]));
};

module TextIcon = {
  module Styles = {
    let iconOuter = Css.(style([marginTop(`em(-0.25))]));
    let icon =
      Css.(style([color(`hex("2a456c")), marginRight(`em(0.5))]));
  };
  [@react.component]
  let make = (~icon, ~text) =>
    ForetoldComponents.(
      <Div
        flexDirection=`row
        justifyContent=`spaceAround
        alignItems=`center
        alignContent=`stretch>
        <Div flex={`num(1.)} className=Styles.iconOuter>
          <ReactKitIcon icon className=Styles.icon />
        </Div>
        <Div flex={`num(1.)}> {text |> Utils.ste} </Div>
      </Div>
    );
};

// @todo: Make a component.
let headerButton = (~onClick, ~icon, ~text) =>
  ForetoldComponents.(
    <Button size=Button.Small className=Styles.actionButtonPosition onClick>
      <TextIcon icon text />
    </Button>
  );

// @todo: Make a component.
let headerLink = (~icon, ~text, ~href) =>
  ForetoldComponents.(
    <Button.Link size=Button.Small className=Styles.actionButtonPosition href>
      <TextIcon icon text />
    </Button.Link>
  );

let make = (~notebookRedux: NotebookRedux.t) => {
  switch (notebookRedux.state.selectedMeasurableId) {
  | Some(measurableId) =>
    Some(
      MeasurableGet.component(~id=measurableId)
      |> E.F.apply((measurable: Types.measurable) => {
           let defaultValueText =
             measurable.recentMeasurement
             |> E.O.bind(_, (r: Types.measurement) => r.valueText)
             |> E.O.default("");

           <>
             <SLayout container=`fluidLeft>
               <ForetoldComponents.PageCard.Body>
                 <div className=Styles.topActions>
                   <div className=Styles.actionButtonsInner>
                     <div className=Styles.actionButtonsRight>
                       {headerLink(
                          ~href=
                            Routing.Url.toString(
                              MeasurableShow(
                                measurable.channelId,
                                measurable.id,
                              ),
                            ),
                          ~icon="REPLY",
                          ~text="Open",
                        )}
                       {headerButton(
                          ~onClick=
                            _e =>
                              notebookRedux.dispatch(DeselectMeasurableId),
                          ~icon="CLOSE",
                          ~text="Close",
                        )}
                     </div>
                   </div>
                 </div>
                 <MeasurementForm
                   measurable
                   defaultValueText
                   key={measurable.id}
                 />
               </ForetoldComponents.PageCard.Body>
             </SLayout>
             <SLayout.Container container=`fluidLeft>
               <MeasurableBottomSection.Inner
                 measurable
                 key={measurable.id}
                 block=`inside
                 leaderboardColumns=LeaderboardTable.Columns.notebooks
               />
             </SLayout.Container>
           </>;
         }),
    )
  | None => None
  };
};