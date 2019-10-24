[@bs.config {jsx: 3}];

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

[@react.component]
let make = (~id: string) => {
  <Providers.AppContext.Consumer>
    ...{({loggedUser}) => {
      let userAgentId =
        loggedUser
        |> E.O.fmap((loggedUser: Types.user) =>
             loggedUser.agent |> E.O.fmap((r: Types.agent) => r.id)
           );

      MeasurableGet.component(~id)
      |> E.F.apply((measurable: Types.measurable) => {
           let creatorId =
             measurable.creator |> E.O.fmap((r: Types.agent) => r.id);

           let form =
             userAgentId == Some(creatorId)
             || Primary.Measurable.toStatus(measurable) !== `JUDGED
               ? <MeasurementForm
                   measurable
                   measurableId=id
                   isCreator={userAgentId == Some(creatorId)}
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
    }}
  </Providers.AppContext.Consumer>;
};
