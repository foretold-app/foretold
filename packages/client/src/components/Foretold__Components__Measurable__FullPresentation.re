open Foretold__GraphQL;
open Style.Grid;
open Utils;

module StatusDisplay = Foretold__Component__StatusDisplay;
module Items = Foretold__Components__Measurable__Items;

let component = ReasonReact.statelessComponent("MeasurableFullPresentation");

module Styles = {
  open Css;
  let header =
    style([
      backgroundColor(`hex("f5f7f9")),
      padding2(~v=`px(10), ~h=`px(13)),
      border(`px(1), `solid, `hex("e8f2f9")),
      borderRadius(`px(3)),
      marginBottom(`px(10)),
    ]);
};

let make = (~id: string, ~loggedInUser: Context.Primary.User.t, _children) => {
  ...component,
  render: _self =>
    Queries.MeasurableWithMeasurements.component(~id)
    |> E.F.apply(m =>
         <>
           <Div flexDirection=`column styles=[Styles.header]>
             <Div flex=1>
               <Div flexDirection=`row>
                 <Div flex=6>
                   {Items.link(~m) |> E.React.inH2}
                   {Items.description(~m) |> E.O.React.defaultNull}
                 </Div>
                 <Div flex=1>
                   <StatusDisplay
                     measurable=m
                     dateDisplay=WHOLE
                     withStatusColorSurrounding=true
                   />
                 </Div>
               </Div>
             </Div>
             <Div flex=1>
               {Items.series(~m) |> E.O.React.defaultNull}
               {Items.creatorLink(~m) |> E.O.React.defaultNull}
               {Items.resolutionEndpoint(~m) |> E.O.React.defaultNull}
               {Items.endpointResponse(~m) |> E.O.React.defaultNull}
             </Div>
           </Div>
           <>
             {
               let userAgentId =
                 loggedInUser.agent
                 |> E.O.fmap((r: Context.Primary.Agent.t) => r.id);
               let creatorId =
                 m.creator |> E.O.fmap((r: Context.Primary.Agent.t) => r.id);
               <>
                 {"Add a Prediction" |> ste |> E.React.inH2}
                 <Foretold__Components__Measurement__Form
                   measurableId=id
                   isCreator={userAgentId == creatorId}
                 />
               </>;
             }
             {"Predictions" |> ste |> E.React.inH2}
             {
               Queries.Measurements.component(
                 ~measurableId=m.id,
                 ~pageLimit=20,
                 ~direction=None,
                 ~innerComponentFn=(
                                     m:
                                       option(
                                         Context.Primary.Connection.t(
                                           Context.Primary.Measurement.t,
                                         ),
                                       ),
                                   ) =>
                 m
                 |> E.O.React.fmapOrNull(
                      (
                        b:
                          Context.Primary.Connection.t(
                            Context.Primary.Measurement.t,
                          ),
                      ) =>
                      b.edges
                      |> E.A.to_list
                      |> Foretold__Components__Measurements__Table.make
                    )
               )
             }
           </>
         </>
       ),
};