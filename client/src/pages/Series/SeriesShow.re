open Utils;
open Rationale;
open Queries;
open HandsOnTable;
open MeasurableColumns;
open Style.Grid;

let transformations = [
  link,
  nameAsText,
  measurementCount,
  measurerCount,
  expectedResolutionDate,
  creator,
];

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

type state = {selected: option(string)};

type action =
  | UpdateSelected(string);

let component = ReasonReact.reducerComponent("Measurables");

let make = (~channel: string, ~id: string, _children) => {
  ...component,
  initialState: () => {selected: None},
  reducer: (action, state) =>
    switch (action) {
    | UpdateSelected(str) => ReasonReact.Update({selected: Some(str)})
    },
  render: ({state, send}) => {
    open Result.Infix;
    let seriesQuery = Queries.GetSeries.Query.make(~id, ());
    let query =
      Queries.GetMeasurables.Query.make(
        ~offset=0,
        ~limit=200,
        ~channel,
        ~seriesId=id,
        (),
      );
    let top =
      Queries.GetSeries.QueryComponent.make(
        ~variables=seriesQuery##variables, series =>
        series.result
        |> ApolloUtils.apolloResponseToResult
        <$> (d => d##series)
        <$> (
          r =>
            switch (r) {
            | Some(k) =>
              <Div styles=[Style.Grid.Styles.flexColumn, Styles.header]>
                <Div styles=[Style.Grid.Styles.flex(1)]>
                  <Div styles=[Style.Grid.Styles.flexRow]>
                    <Div styles=[Style.Grid.Styles.flex(6)]>
                      <h2>
                        <Icon.Icon icon="LAYERS" />
                        {k.name |> Option.default("") |> ste}
                      </h2>
                      <p> {k.description |> Option.default("") |> ste} </p>
                    </Div>
                    <Div styles=[Style.Grid.Styles.flex(1)] />
                  </Div>
                </Div>
                <Div styles=[Style.Grid.Styles.flex(1)] />
              </Div>
            | None => <div />
            }
        )
        |> Result.result(idd, idd)
      );
    let bottom =
      Queries.GetMeasurables.QueryComponent.make(
        ~variables=query##variables, o =>
        o.result
        |> ApolloUtils.apolloResponseToResult
        <$> (d => d##measurables)
        <$> Extensions.Array.concatSomes
        <$> (d => d |> Array.map(Queries.GetMeasurables.toMeasurable))
        <$> (
          measurables =>
            <SeriesShowTable
              measurables
              selected={state.selected}
              onClick={id => send(UpdateSelected(id))}
            />
        )
        |> Result.result(idd, idd)
      );

    let lower =
      state.selected
      |> Rationale.Option.fmap(idd =>
           Queries.GetMeasurableWithMeasurements.withQuery(
             ~id=idd,
             measurable => {
               let m =
                 Queries.GetMeasurableWithMeasurements.queryMeasurable(
                   measurable,
                 );
               <div>
                 <Div styles=[Style.Grid.Styles.flexColumn, Styles.header]>
                   <Div styles=[Style.Grid.Styles.flex(1)]>
                     <Div styles=[Style.Grid.Styles.flexRow]>
                       <Div styles=[Style.Grid.Styles.flex(6)]>
                         <h2> {MeasurableTableStyles.link(~m)} </h2>
                         {MeasurableTableStyles.description(~m)}
                       </Div>
                       <Div styles=[Style.Grid.Styles.flex(1)]>
                         {
                           MeasurableTableStyles.dateStatusWrapper(
                             ~measurable=m,
                           )
                         }
                       </Div>
                     </Div>
                   </Div>
                   <Div styles=[Style.Grid.Styles.flex(1)]>
                     {MeasurableTableStyles.creatorLink(~m)}
                     {MeasurableTableStyles.resolutionEndpoint(~m)}
                     {MeasurableTableStyles.endpointResponse(~m)}
                   </Div>
                 </Div>
                 <div>
                   {
                     SharedQueries.withLoggedInUserQuery(userQuery =>
                       switch (userQuery) {
                       | Some(query) =>
                         open Rationale.Option.Infix;
                         let userAgentId = query##user >>= (e => e##agentId);
                         let creatorId = measurable##creatorId;
                         <div>
                           <h2> {"Add a Measurement" |> ste} </h2>
                           <MeasurableShowForm
                             measurableId=idd
                             isCreator={userAgentId == creatorId}
                           />
                         </div>;
                       | _ => <div />
                       }
                     )
                   }
                   <h2> {"Measurements" |> ste} </h2>
                   <Measurable__Table measurements=measurable##measurements />
                 </div>
               </div>;
             },
           )
         )
      |> Option.default(ReasonReact.null);
    <div>
      {top |> ReasonReact.element}
      <div className=SeriesShowTableStyles.topPart>
        {bottom |> ReasonReact.element}
      </div>
      lower
    </div>
    |> FillWithSidebar.make(~channel=Some(channel))
    |> ReasonReact.element;
  },
};