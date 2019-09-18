open Style.Grid;

module Styles = {
  open Css;

  let header =
    style(
      [padding2(~v=`em(1.5), ~h=`em(1.5))]
      @ FC.Base.BaseStyles.fullWidthFloatLeft,
    );

  let link = style([marginTop(em(1.))]);

  let description = style([paddingTop(`em(1.5))]);
};

let component = ReasonReact.statelessComponent("Measurable");
let make = (~id: string, ~loggedInUser: Types.user, _children) => {
  ...component,
  render: _self => {
    let userAgentId =
      loggedInUser.agent |> E.O.fmap((r: Types.agent) => r.id);

    MeasurableGet2.component(~id)
    |> E.F.apply((measurable: Types.measurable) => {
         let creatorId =
           measurable.creator |> E.O.fmap((r: Types.agent) => r.id);

         let form =
           userAgentId == creatorId
           || Primary.Measurable.toStatus(measurable) !== `JUDGED
             ? <MeasurementForm
                 measurable
                 measurableId=id
                 isCreator={userAgentId == creatorId}
                 loggedInUser
               />
             : E.React.null;

         <>
           <Div styles=[Styles.header]>
             <Div flexDirection=`row>
               <Div flex={`num(3.)}>
                 <FC.PageCard.H1>
                   {MeasurableItems.link(~m=measurable)}
                 </FC.PageCard.H1>
                 <StatusDisplay measurable />
               </Div>
               <Div flex={`num(1.)}>
                 {MeasurableItems.series(~m=measurable, ())
                  |> E.O.React.defaultNull}
                 {MeasurableItems.creatorLink(~m=measurable)
                  |> E.O.React.defaultNull}
                 {E.React.showIf(
                    Primary.Permissions.can(
                      `MEASURABLE_UPDATE,
                      measurable.permissions,
                    ),
                    MeasurableItems.editLink(~m=measurable),
                  )}
                 {MeasurableItems.resolutionEndpoint(~m=measurable)
                  |> E.O.React.defaultNull}
                 {MeasurableItems.endpointResponse(~m=measurable)
                  |> E.O.React.defaultNull}
                 {MeasurableItems.questionLink(~m=measurable)}
               </Div>
             </Div>
             {MeasurableItems.description(~m=measurable)
              |> E.O.React.fmapOrNull(d =>
                   <Div styles=[Styles.description]> d </Div>
                 )}
           </Div>
           form
         </>;
       });
  },
};