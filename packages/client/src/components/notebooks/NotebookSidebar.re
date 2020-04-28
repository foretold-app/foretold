module Styles = {
  let topActions = Css.(style([display(`flex), padding(`em(0.2))]));
  let actionButtonPosition =
    Css.(style([float(`right), marginLeft(`em(1.))]));
  let actionButtonsInner = Css.(style([width(`percent(100.0))]));
  let actionButtonsRight = Css.(style([float(`right)]));
  let iconOuter = Css.(style([marginTop(`em(-0.25))]));
  let icon = Css.(style([color(`hex("2a456c")), marginRight(`em(0.5))]));
};

let headerButton = (~onClick, ~icon, ~text) =>
  ForetoldComponents.(
    <Button size=Button.Small className=Styles.actionButtonPosition onClick>
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
    </Button>
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
                       {headerButton(
                          ~onClick=
                            _e =>
                              Routing.Url.push(
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