open Utils;
open Style.Grid;
open Foretold__GraphQL;

module Items = Foretold__Components__Measurable__Items;

module StatusDisplay = {
  let formatDate = e =>
    e |> E.O.fmap(E.M.format(E.M.format_simple)) |> E.O.default("");

  type dateDisplay =
    | TOP
    | BOTTOM
    | WHOLE;

  type status =
    | OPEN
    | PENDING_REVIEW
    | ARCHIVED
    | JUDGED;

  let dateFinder = (head, p, date, dateDisplay) => {
    let date = formatDate(date);
    switch (dateDisplay) {
    | TOP => head |> ste
    | BOTTOM => p ++ date |> ste
    | WHOLE =>
      <div className=PrimaryTableStyles.statusRow>
        <h3> {head |> ste} </h3>
        <p> {p ++ date |> ste} </p>
      </div>
    };
  };

  let statusShow = (~measurable: DataModel.Measurable.t, ~dateDisplay) =>
    switch (DataModel.Measurable.toStatus(measurable)) {
    | OPEN =>
      dateFinder(
        "Open",
        "Closes ~",
        measurable.expectedResolutionDate,
        dateDisplay,
      )
    | PENDING_REVIEW =>
      dateFinder(
        "Judgement Pending",
        "Pending since ",
        measurable.expectedResolutionDate,
        dateDisplay,
      )
    | ARCHIVED =>
      dateFinder(
        "Archived",
        "Archived on ",
        measurable.stateUpdatedAt,
        dateDisplay,
      )
    | JUDGED =>
      dateFinder(
        "Judged",
        "Judged on ",
        measurable.stateUpdatedAt,
        dateDisplay,
      )
    };
  let component = ReasonReact.statelessComponent("MeasurableShow");

  let make =
      (
        ~measurable: DataModel.Measurable.t,
        ~dateDisplay=WHOLE,
        ~withStatusColorSurrounding=false,
        _children,
      ) => {
    ...component,
    render: _self => {
      let finder = statusShow(~measurable, ~dateDisplay);
      true ?
        <div className={PrimaryTableStyles.statusColor(~measurable)}>
          finder
        </div> :
        finder;
    },
  };
};

module FullPresentation = {
  let component =
    ReasonReact.statelessComponent("MeasurableFullPresentation");

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
};