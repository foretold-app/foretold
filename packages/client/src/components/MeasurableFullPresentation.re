open Style.Grid;

module StatusDisplay = Foretold__Component__StatusDisplay;
module Items = Foretold__Components__Measurable__Items;

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

let component = ReasonReact.statelessComponent("MeasurableFullPresentation");
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
             ? <Foretold__Components__Measurement__Form
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
                   {Items.link(~m=measurable)}
                 </FC.PageCard.H1>
                 <StatusDisplay measurable />
               </Div>
               <Div flex={`num(1.)}>
                 {Items.series(~m=measurable, ()) |> E.O.React.defaultNull}
                 {Items.creatorLink(~m=measurable) |> E.O.React.defaultNull}
                 {E.React.showIf(
                    Primary.Permissions.can(
                      `MEASURABLE_UPDATE,
                      measurable.permissions,
                    ),
                    Items.editLink(~m=measurable),
                  )}
                 {Items.resolutionEndpoint(~m=measurable)
                  |> E.O.React.defaultNull}
                 {Items.endpointResponse(~m=measurable)
                  |> E.O.React.defaultNull}
                 {Items.questionLink(~m=measurable)}
               </Div>
             </Div>
             {Items.description(~m=measurable)
              |> E.O.React.fmapOrNull(d =>
                   <Div styles=[Styles.description]> d </Div>
                 )}
           </Div>
           form
         </>;
       });
  },
};