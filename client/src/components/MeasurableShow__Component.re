open Utils;
open Style.Grid;

let component = ReasonReact.statelessComponent("MeasurableShow");

module Styles = {
  open Css;
  let sidebar =
    style([Css.float(`left), left(px(0)), backgroundColor(hex("eee"))]);

  let body = style([marginLeft(px(200)), padding(px(30))]);

  let header =
    style([
      backgroundColor(hex("f5f7f9")),
      padding2(~v=`px(10), ~h=`px(13)),
      border(`px(1), `solid, `hex("e8f2f9")),
      borderRadius(`px(3)),
      marginBottom(`px(10)),
    ]);
};

let make = (~id: string, ~loggedInUser: GetUser.t, _children) => {
  ...component,
  render: _self =>
    GetMeasurableWithMeasurements.component(
      ~id,
      measurable => {
        let m = GetMeasurableWithMeasurements.queryMeasurable(measurable);
        <div>
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
              {MeasurableTableStyles.series(~m)}
              {MeasurableTableStyles.creatorLink(~m)}
              {MeasurableTableStyles.resolutionEndpoint(~m)}
              {MeasurableTableStyles.endpointResponse(~m)}
            </Div>
          </Div>
          <div>
            {
              loggedInUser
              |> E.O.fmap((user: GetUser.user) => {
                   open Rationale.Option.Infix;
                   let userAgentId = user.agentId;
                   let creatorId = m.creator <$> (r => r.id);
                   <div>
                     <h2> {"Add a Measurement" |> ste} </h2>
                     <MeasurableShowForm
                       measurableId=id
                       isCreator={userAgentId == creatorId}
                     />
                   </div>;
                 })
              |> E.O.React.defaultNull
            }
            <h2> {"Measurements" |> ste} </h2>
            <Measurable__Table measurements=measurable##measurements />
          </div>
        </div>;
      },
    ),
};