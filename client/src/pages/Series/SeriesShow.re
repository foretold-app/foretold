open Utils;
open Rationale;
open Queries;
open Style.Grid;

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

let seriesTop = (series: Queries.GetSeries.series) =>
  <Div styles=[Style.Grid.Styles.flexColumn, Styles.header]>
    <Div styles=[Style.Grid.Styles.flex(1)]>
      <Div styles=[Style.Grid.Styles.flexRow]>
        <Div styles=[Style.Grid.Styles.flex(6)]>
          <h2>
            <Icon.Icon icon="LAYERS" />
            {series.name |> Option.default("") |> ste}
          </h2>
          <p> {series.description |> Option.default("") |> ste} </p>
        </Div>
        <Div styles=[Style.Grid.Styles.flex(1)] />
      </Div>
    </Div>
    <Div styles=[Style.Grid.Styles.flex(1)] />
  </Div>;

let make = (~channel: string, ~id: string, ~userQuery, _children) => {
  ...component,
  initialState: () => {selected: None},
  reducer: (action, state) =>
    switch (action) {
    | UpdateSelected(str) => ReasonReact.Update({selected: Some(str)})
    },
  render: ({state, send}) => {
    open Result.Infix;
    let top =
      Queries.GetSeries.component(~id, series =>
        switch (series) {
        | Some(k) => seriesTop(k)
        | None => ReasonReact.null
        }
      );

    let bottom =
      Queries.GetMeasurables.componentWithSeries(channel, id, measurables =>
        <SeriesShowTable
          measurables
          selected={state.selected}
          onClick={id => send(UpdateSelected(id))}
        />
      );

    let lower =
      state.selected
      |> E.O.fmap(idd =>
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
                     switch (userQuery) {
                     | Some(query) =>
                       let userAgentId =
                         query##user |> E.O.bind(_, e => e##agentId);
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
      top
      <div className=SeriesShowTableStyles.topPart> bottom </div>
      lower
    </div>;
  },
};