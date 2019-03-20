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

let make = (~id: string, ~loggedInUser: Queries.User.t, _children) => {
  ...component,
  render: _self =>
    Queries.MeasurableWithMeasurements.component(~id)
    |> E.F.apply(m =>
         <>
           <Div flexDirection=`column styles=[Styles.header]>
             <Div flex=1>
               <Div flexDirection=`row>
                 <Div flex=6>
                   <h2> {Items.link(~m)} </h2>
                   {Items.description(~m)}
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
               {
                 [|
                   Items.series(~m),
                   Items.creatorLink(~m),
                   Items.resolutionEndpoint(~m),
                   Items.endpointResponse(~m),
                 |]
                 |> ReasonReact.array
               }
             </Div>
           </Div>
           <>
             {
               loggedInUser
               |> E.O.fmap((user: Queries.User.user) => {
                    let userAgentId = user.agentId;
                    let creatorId =
                      m.creator |> E.O.fmap((r: DataModel.Agent.t) => r.id);
                    <>
                      <h2> {"Add a Measurement" |> ste} </h2>
                      <MeasurableShowForm
                        measurableId=id
                        isCreator={userAgentId == creatorId}
                      />
                    </>;
                  })
               |> E.O.React.defaultNull
             }
             <h2> {"Measurements" |> ste} </h2>
             {
               m.measurements
               |> E.O.fmap(measurements =>
                    measurements
                    |> Foretold__Components__Measurements__Table.make
                  )
               |> E.O.React.defaultNull
             }
           </>
         </>
       ),
};