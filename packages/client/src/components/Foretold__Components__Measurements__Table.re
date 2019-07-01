open Utils;
open MomentRe;
open Style.Grid;
open Css;

type measurement = Context.Primary.Measurement.t;
type measurable = Context.Primary.Measurable.t;
module Items = Foretold__Components__Measurable__Items;

module Styles = {
  open Css;

  let middle =
    style([
      textAlign(`center),
      fontSize(`em(1.4)),
      fontWeight(`num(800)),
      color(`hex("7d7ea2")),
      marginTop(`px(6)),
    ]);

  let descriptionStyle =
    style([
      marginTop(`px(5)),
      selector(
        " p",
        [color(`rgba((80, 80, 80, 0.78))), marginBottom(`px(10))],
      ),
    ]);

  let date = style([color(`rgba((35, 35, 35, 0.34)))]);

  let agentStyle =
    style([color(`rgb((102, 121, 134))), fontSize(`em(1.1))]);

  let mainColumn =
    style([flex(4), display(`flex), flexDirection(`column)]);

  let mainColumnTop =
    style([
      flex(1),
      paddingLeft(px(2)),
      selector(" p", [marginTop(`px(3)), marginBottom(`px(8))]),
    ]);

  let rightColumn = style([flex(1)]);

  let mainColumnBottom =
    style([flex(1), padding(`px(2)), marginTop(`px(2))]);

  let secondaryText =
    style([fontSize(`em(0.9)), color(FC__Colors.accentBlue)]);
};

module Helpers = {
  let smallDistribution = (measurement: measurement, g: (float, float)) =>
    switch (measurement.value) {
    | Ok(`FloatCdf(r)) =>
      let (minX, maxX) = g;
      r
      |> MeasurementValue.toChunks(~bucketSize=3)
      |> MeasurementValue.toPdf
      |> MeasurementValue.FloatCdf.toJs
      |> (
        data =>
          Some(
            <SmallCdfChart
              data
              minX
              maxX
              color={
                measurement.competitorType == `AGGREGATION
                  ? `hex("b1b9c6") : `hex("487192")
              }
            />,
          )
      );
    | Ok(`FloatPoint(r)) =>
      Some(
        <div className=Styles.middle>
          {r |> E.Float.with3DigitsPrecision |> ste}
        </div>,
      )
    | _ => None
    };

  let statSummary = (measurement: measurement) =>
    switch (measurement.value) {
    | Ok(`FloatCdf(r)) =>
      r
      |> MeasurementValue.toChunks(~bucketSize=3)
      |> MeasurementValue.FloatCdf.toJs
      |> FC.Base.Types.Dist.fromJson
      |> (cdf => Some(<FC__CdfChart__StatSummary cdf />))
    | Ok(`FloatPoint(r)) => Some(r |> E.Float.with3DigitsPrecision |> ste)
    | _ => None
    };

  let getValueText = (measurement: measurement) =>
    switch (measurement.value) {
    | Ok(`FloatCdf(_r)) =>
      Some(
        <div className=Styles.secondaryText>
          {measurement.valueText |> E.O.default("") |> Utils.ste}
        </div>,
      )
    | _ => None
    };

  let description = (~m: measurement) =>
    switch (m.description) {
    | None
    | Some("") => None
    | Some(description) =>
      Some(
        <div className=Styles.descriptionStyle>
          <ReactMarkdown.Markdown source=description />
        </div>,
      )
    };

  let relevantAt = (~m: measurement) =>
    m.relevantAt
    |> E.O.fmap(d =>
         <div className=Styles.date> {d |> E.M.goFormat_standard |> ste} </div>
       );

  let getFloatCdf = (e: Belt.Result.t(MeasurementValue.t, string)) =>
    switch (e) {
    | Ok(`FloatCdf(r)) => Some(r)
    | _ => None
    };

  let bounds = (measurement: Js_array.t(measurement)) => {
    let itemBounds =
      measurement
      |> E.A.keepMap(_, r => getFloatCdf(r.value))
      |> E.A.fmap(r =>
           (
             E.FloatCdf.firstAboveValue(0.05, r),
             E.FloatCdf.firstAboveValue(0.95, r),
           )
         );

    let min =
      itemBounds
      |> E.A.keepMap(_, ((min, _)) => min)
      |> E.A.fold_left((a, b) => a < b ? a : b, max_float);

    let max =
      itemBounds
      |> E.A.keepMap(_, ((_, max)) => max)
      |> E.A.fold_left((a, b) => a > b ? a : b, min_float);

    (min, max);
  };

  let toChartMeasurement = (measurement: measurement) =>
    switch (measurement.value) {
    | Ok(`FloatCdf(r)) =>
      switch (
        E.FloatCdf.firstAboveValue(0.05, r),
        E.FloatCdf.firstAboveValue(0.95, r),
      ) {
      | (Some(low), Some(high)) => Some((low, high))
      | _ => None
      }
    | _ => None
    };

  let measurerLink = (~measurement: measurement) => {
    let aLink =
      switch (
        measurement.agent,
        measurement.agent |> E.O.bind(_, Context.Primary.Agent.name),
      ) {
      | (Some(agent), Some(name)) =>
        Some(
          <Foretold__Components__Link
            linkType={
              Internal(
                Agent({agentId: agent.id, subPage: AgentMeasurements}),
              )
            }
            className=Styles.agentStyle>
            {name |> ste}
          </Foretold__Components__Link>,
        )
      | _ => None
      };

    let judgementStyle =
      style([
        selector(
          " h3",
          [
            color(`rgba((55, 47, 68, 0.85))),
            marginBottom(`px(0)),
            fontSize(`em(1.15)),
            fontWeight(`num(800)),
          ],
        ),
        selector(" a", [fontSize(`em(0.9))]),
      ]);

    let isJudge = Context.Primary.Measurement.isJudgement(measurement);

    if (isJudge) {
      <div className=judgementStyle>
        {"Resolution" |> ste |> E.React.inH3}
        {switch (aLink) {
         | Some(name) => <> {"by " |> ste} name </>
         | None => E.React.null
         }}
      </div>;
    } else {
      aLink |> E.O.React.defaultNull;
    };
  };
};

let stringOfFloat = Js.Float.toPrecisionWithPrecision(_, ~digits=3);

let primaryCellStyle =
  Css.(style([paddingTop(`em(0.6)), paddingBottom(`em(0.0))]));

let headerCellStyle =
  Css.(style([paddingTop(`em(0.7)), paddingBottom(`em(0.7))]));

let sort = (a: measurement, b: measurement) =>
  switch (a.relevantAt, b.relevantAt, a.competitorType, b.competitorType) {
  | (Some(c), Some(d), _, _) when Moment.toUnix(c) < Moment.toUnix(d) => 1
  | (Some(c), Some(d), `AGGREGATION, `COMPETITIVE)
      when Moment.toUnix(c) == Moment.toUnix(d) => (-1)
  | (Some(c), Some(d), `COMPETITIVE, `AGGREGATION)
      when Moment.toUnix(c) == Moment.toUnix(d) => 1
  | (Some(c), Some(d), _, _) when Moment.toUnix(c) == Moment.toUnix(d) => 0
  | (Some(c), Some(d), _, _) when Moment.toUnix(c) > Moment.toUnix(d) => (-1)
  | _ => 0
  };

let getItems = (measurementsList: list(measurement), ~makeItem) => {
  let _bounds = Helpers.bounds(measurementsList |> E.A.of_list);
  measurementsList
  |> E.L.sort(sort)
  |> E.L.fmap((m: measurement) => {
       let inside = makeItem(m, _bounds);

       switch (Helpers.description(~m)) {
       | Some(description) =>
         <FC.Table.Row
           bottomSubRow=[|FC.Table.Row.textSection(description)|]>
           inside
         </FC.Table.Row>
       | None => <FC.Table.Row> inside </FC.Table.Row>
       };
     })
  |> E.A.of_list
  |> ReasonReact.array;
};

let getItemsSorted = (measurementsList: list(measurement), ~makeItem) => {
  let _bounds = Helpers.bounds(measurementsList |> E.A.of_list);

  measurementsList
  |> E.L.sort(sort)
  |> E.L.fmap((measurement: measurement) => makeItem(measurement, _bounds))
  |> E.A.of_list
  |> ReasonReact.array;
};

let getMeasurableLink = (m: measurement) => {
  switch (m.measurable) {
  | None => "" |> ste
  | Some(measurable) => Items.link(~m=measurable)
  };
};

type column = Table.column(Context.Primary.Measurement.t);

let predictionValueColumn: column =
  Table.Column.make(
    ~name="Prediction Value" |> ste,
    ~render=
      (measurement: Context.Primary.Measurement.t) =>
        <div>
          {Helpers.statSummary(measurement) |> E.O.React.defaultNull}
          {Helpers.getValueText(measurement) |> E.O.React.defaultNull}
        </div>,
    (),
  );

let agentColumn: column =
  Table.Column.make(
    ~name="Agent" |> ste,
    ~render=
      (measurement: Context.Primary.Measurement.t) =>
        Helpers.measurerLink(~measurement),
    (),
  );

let timeColumn: column =
  Table.Column.make(
    ~name="Time" |> ste,
    ~render=
      (measurement: Context.Primary.Measurement.t) =>
        Helpers.relevantAt(~m=measurement) |> E.O.React.defaultNull,
    (),
  );

let mesurableColumn: column =
  Table.Column.make(
    ~name="Measurable" |> ste,
    ~render=
      (measurement: Context.Primary.Measurement.t) =>
        <div
          className=Css.(
            style([paddingTop(`em(1.0)), paddingBottom(`em(0.5))])
          )>
          <div className=Styles.mainColumn>
            <div className=Styles.mainColumnTop>
              {getMeasurableLink(measurement)}
            </div>
          </div>
        </div>,
    ~flex=2,
    (),
  );

let getPredictionDistributionColumn = (bounds): column =>
  Table.Column.make(
    ~name="Prediction Distribution" |> ste,
    ~render=
      (measurement: Context.Primary.Measurement.t) =>
        Helpers.smallDistribution(measurement, bounds)
        |> E.O.React.defaultNull,
    ~show=
      (measurement: Context.Primary.Measurement.t) =>
        switch (measurement.measurable) {
        | Some(measurable) =>
          Js.log2(
            "measurable.valueType !== `PERCENTAGE",
            measurable.valueType |> Context.Primary.Measurable.valueTypeToStr,
          );
          measurable.valueType !== `PERCENTAGE;
        | _ => true
        },
    ~flex=2,
    (),
  );

let make = (measurementsList: list(measurement)) => {
  let bounds = Helpers.bounds(measurementsList |> E.A.of_list);

  let all: array(column) = [|
    getPredictionDistributionColumn(bounds),
    predictionValueColumn,
    agentColumn,
    timeColumn,
  |];

  let measurementsList' = measurementsList |> E.L.sort(sort);

  measurementsList' |> E.L.length > 0
    ? <FC.PageCard.Body>
        {Table.fromColumns(all, measurementsList' |> Array.of_list, ())}
      </FC.PageCard.Body>
    : <SLayout.NothingToShow />;
};

let makeAgentPredictionsTable =
    (
      ~measurementsList: list(measurement),
      ~onSelect=(_measurement: Context.Primary.Measurement.t) => (),
      (),
    ) => {
  let bounds = Helpers.bounds(measurementsList |> E.A.of_list);

  let all: array(column) = [|
    mesurableColumn,
    getPredictionDistributionColumn(bounds),
    predictionValueColumn,
    timeColumn,
  |];

  let measurementsList' = measurementsList |> E.L.sort(sort);

  let onRowClb = (measurement: Context.Primary.Measurement.t) => {
    onSelect(measurement);
    ();
  };

  measurementsList' |> E.L.length > 0
    ? <FC.PageCard.Body>
        {Table.fromColumns(
           all,
           measurementsList' |> Array.of_list,
           ~onRowClb=Some(onRowClb),
           (),
         )}
      </FC.PageCard.Body>
    : <SLayout.NothingToShow />;
};