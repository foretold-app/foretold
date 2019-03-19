open Utils;
open Style.Grid;
open Foretold__GraphQL;

let component = ReasonReact.statelessComponent("MeasurableShow");

module Styles = {
  open Css;
  let header =
    style([
      backgroundColor(hex("f5f7f9")),
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
                   <h2> {MeasurableTableStyles.link(~m)} </h2>
                   {MeasurableTableStyles.description(~m)}
                 </Div>
                 <Div flex=1>
                   {MeasurableTableStyles.dateStatusWrapper(~measurable=m)}
                 </Div>
               </Div>
             </Div>
             <Div flex=1>
               {
                 [|
                   MeasurableTableStyles.series(~m),
                   MeasurableTableStyles.creatorLink(~m),
                   MeasurableTableStyles.resolutionEndpoint(~m),
                   MeasurableTableStyles.endpointResponse(~m),
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
                    <div className=MeasurementTableStyles.group>
                      {measurements |> MeasurementsBlock.make}
                    </div>
                  )
               |> E.O.React.defaultNull
             }
           </>
         </>
       ),
};