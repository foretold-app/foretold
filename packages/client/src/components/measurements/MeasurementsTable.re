open Utils;
open MomentRe;

type measurement = Types.measurement;
type measurable = Types.measurable;
module Items = MeasurableItems;

module Styles = {
  open Css;

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

  let secondaryText =
    style([fontSize(`em(0.9)), color(FC__Settings.accentBlue)]);

  let percentage = style([fontSize(`em(1.15))]);

  let result =
    style([
      fontSize(`em(1.15)),
      fontWeight(`num(800)),
      color(`hex("7d7ea2")),
    ]);
};

module Helpers = {
  open Css;
  let smallDistribution =
      (
        ~measurement: measurement,
        ~bounds: (float, float),
        ~width=230,
        ~height=38,
        (),
      )
      : option(React.element) =>
    switch (measurement.value) {
    | Ok(`FloatCdf(r)) =>
      let (minX, maxX) = bounds;
      r
      |> MeasurementValue.toPdf
      |> MeasurementValue.FloatCdf.toJs
      |> (
        data =>
          Some(
            <SmallCdfChart
              minX={Some(minX)}
              maxX={Some(maxX)}
              data
              width
              height
              color={
                measurement.competitorType == `AGGREGATION
                  ? `hex("b1b9c6") : `hex("487192")
              }
            />,
          )
      );
    | Ok(`FloatPoint(r)) =>
      Some(
        <div className=Styles.result>
          {r |> E.Float.with3DigitsPrecision |> ste}
        </div>,
      )
    | _ => None
    };

  let statSummary = (measurement: measurement): option(React.element) =>
    switch (measurement.value) {
    | Ok(`FloatCdf(r)) =>
      r
      |> MeasurementValue.FloatCdf.toJs
      |> FC.Base.Types.Dist.fromJson
      |> (cdf => Some(<FC__CdfChart__StatSummary cdf showMean=false />))
    | Ok(`FloatPoint(r)) => Some(<FC__NumberShower precision=8 number=r />)
    | Ok(`Percentage(r)) =>
      Some(
        <span className=Styles.percentage>
          <FC__PercentageShower precision=8 percentage=r />
        </span>,
      )
    | Ok(`Binary(r)) =>
      Some(
        <span className=Styles.result>
          {(r ? "Yes" : "No") |> Utils.ste}
        </span>,
      )
    | Ok(`UnresolvableResolution(r)) =>
      Some(
        <span className=Styles.result>
          {r
           |> MeasurementValue.UnresolvableResolution.toPublicString
           |> Utils.ste}
        </span>,
      )
    | _ => None
    };

  let getValueText = (measurement: measurement): React.element =>
    <div className=Styles.secondaryText>
      {measurement.valueText
       |> E.O.bind(_, r => r == "" ? None : Some(r))
       |> E.O.fmap(r => {|"|} ++ r ++ {|"|})
       |> E.O.default("")
       |> Utils.ste}
    </div>;

  let getDescription = (~m: measurement): option(React.element) => {
    switch (m.description) {
    | None
    | Some("") => None
    | Some(description) =>
      Some(
        <div className=Styles.descriptionStyle>
          <ReactMarkdown source=description />
        </div>,
      )
    };
  };

  let relevantAt = (~m: measurement): option(React.element) =>
    m.relevantAt
    |> E.O.fmap(d =>
         <div className=Styles.date> {d |> E.M.goFormat_standard |> ste} </div>
       );

  let getFloatCdf = (e: Belt.Result.t(MeasurementValue.t, string)) =>
    switch (e) {
    | Ok(`FloatCdf(r)) => Some(r)
    | _ => None
    };

  let bounds = (measurements: Js_array.t(measurement)) => {
    let itemBounds =
      measurements
      |> E.A.keepMap(_, r => getFloatCdf(r.value))
      |> E.A.fmap(r =>
           (
             E.FloatCdf.firstAboveValue(0.02, r),
             E.FloatCdf.firstAboveValue(0.98, r),
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
        E.FloatCdf.firstAboveValue(0.02, r),
        E.FloatCdf.firstAboveValue(0.98, r),
      ) {
      | (Some(low), Some(high)) => Some((low, high))
      | _ => None
      }
    | _ => None
    };

  let measurerLink = (~measurement: measurement) => {
    let aLink = measurement.agent |> E.O.fmap(agent => <AgentLink agent />);

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

    let isJudge = Primary.Measurement.isJudgement(measurement);

    if (isJudge) {
      <div className=judgementStyle>
        {"Resolution" |> ste |> E.React2.inH3}
        {switch (aLink) {
         | Some(name) => <> name </>
         | None => E.React2.null
         }}
      </div>;
    } else {
      aLink |> E.O.React.defaultNull;
    };
  };
};

let stringOfFloat = Js.Float.toPrecisionWithPrecision(_, ~digits=3);

let getItems = (measurementsList: list(measurement), ~makeItem) => {
  let _bounds = Helpers.bounds(measurementsList |> E.A.of_list);
  measurementsList
  |> E.L.fmap((m: measurement) => {
       let inside = makeItem(m, _bounds);

       switch (Helpers.getDescription(~m)) {
       | Some(description) =>
         <FC.Table.Row
           bottomSubRow={
             [|FC.Table.Row.textSection(description)|] |> ReasonReact.array
           }>
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

type column = Table.column(Types.measurement);

let predictionValueColumn =
  Table.Column.make(
    ~name="Prediction" |> ste,
    ~flex=5,
    ~render=
      (measurement: Types.measurement) =>
        <div>
          {Helpers.statSummary(measurement) |> E.O.React.defaultNull}
        </div>,
    (),
  );

let predictionTextColumn =
  Table.Column.make(
    ~name={
      "Input Text" |> ste;
    },
    ~flex=3,
    ~render=
      (measurement: Types.measurement) =>
        <div> {Helpers.getValueText(measurement)} </div>,
    (),
  );

let agentColumn =
  Table.Column.make(
    ~name="Member" |> ste,
    ~flex=5,
    ~render=
      (measurement: Types.measurement) => Helpers.measurerLink(~measurement),
    (),
  );

let timeColumn =
  Table.Column.make(
    ~name="Time" |> ste,
    ~flex=5,
    ~render=
      (measurement: Types.measurement) =>
        Helpers.relevantAt(~m=measurement) |> E.O.React.defaultNull,
    (),
  );

let measurableColumn =
  Table.Column.make(
    ~name="Measurable" |> ste,
    ~render=
      (measurement: Types.measurement) => getMeasurableLink(measurement),
    ~flex=5,
    (),
  );

let scoreColumn = _ =>
  Table.Column.make(
    ~name={
      "Relative Score" |> ste;
    },
    ~help=
      Some({
        headerContent: "Relative Score" |> ste,
        bodyContent:
          "The difference between the log error for this predictions vs. the log score for the most recent aggregate prediction."
          |> ste,
      }),
    ~render=
      (measurement: Types.measurement) =>
        measurement.measurementScoreSet
        |> E.O.ffmap((o: Types.measurementScoreSet) =>
             o.primaryPointScore |> E.O.fmap(E.Float.with3DigitsPrecision)
           )
        |> E.O.default("")
        |> Utils.ste,
    ~flex=5,
    (),
  );

let logScoreColumn = _ =>
  Table.Column.make(
    ~name="Log Score" |> ste,
    ~help=
      Some({
        headerContent: "Log Score" |> ste,
        bodyContent:
          "The absolute log error of the forecast vs. the most recent resolution."
          |> ste,
      }),
    ~render=
      (measurement: Types.measurement) =>
        measurement.measurementScoreSet
        |> E.O.bind(_, (o: Types.measurementScoreSet) =>
             o.nonMarketLogScore |> E.O.fmap(E.Float.with3DigitsPrecision)
           )
        |> E.O.default("")
        |> Utils.ste,
    ~flex=5,
    (),
  );

let getPredictionDistributionColumn = bounds =>
  Table.Column.make(
    ~name="Distribution" |> ste,
    ~flex=7,
    ~render=
      (measurement: Types.measurement) =>
        Helpers.smallDistribution(~measurement, ~bounds, ())
        |> E.O.React.defaultNull,
    ~show=
      (measurement: Types.measurement) =>
        switch (measurement.measurable) {
        | Some(measurable) => measurable.valueType !== `PERCENTAGE
        | _ => true
        },
    (),
  );

let bottomSubRowFn =
  Some(
    (measurement: Types.measurement) =>
      Helpers.getDescription(~m=measurement)
      |> E.O.fmap((c: React.element) => [|FC.Table.Row.textSection(c)|]),
  );

let make =
    (
      ~measurementsList: list(measurement),
      ~measurableValueType: Types.valueType,
      (),
    ) => {
  let bounds = Helpers.bounds(measurementsList |> E.A.of_list);

  let all =
    switch (measurableValueType) {
    | `FLOAT => [|
        agentColumn,
        getPredictionDistributionColumn(bounds),
        predictionValueColumn,
        predictionTextColumn,
        timeColumn,
      |]
    | `PERCENTAGE => [|
        agentColumn,
        getPredictionDistributionColumn(bounds),
        predictionValueColumn,
        timeColumn,
      |]
    | `DATE => Js.Exn.raiseError("Date not supported ")
    };

  let measurementsList' = measurementsList;

  measurementsList' |> E.L.length > 0
    ? <FC.PageCard.Body>
        {Table.fromColumns(
           all,
           measurementsList' |> Array.of_list,
           ~bottomSubRowFn,
           (),
         )}
      </FC.PageCard.Body>
    : <NothingToShow />;
};

let makeExtended =
    (
      ~measurementsList: list(measurement),
      ~measurableValueType: Types.valueType,
      (),
    ) => {
  let bounds = Helpers.bounds(measurementsList |> E.A.of_list);

  let all =
    switch (measurableValueType) {
    | `FLOAT => [|
        agentColumn,
        getPredictionDistributionColumn(bounds),
        predictionValueColumn,
        predictionTextColumn,
        logScoreColumn(),
        scoreColumn(),
        timeColumn,
      |]
    | `PERCENTAGE => [|
        agentColumn,
        getPredictionDistributionColumn(bounds),
        predictionValueColumn,
        logScoreColumn(),
        scoreColumn(),
        timeColumn,
      |]
    | `DATE => Js.Exn.raiseError("Date not supported ")
    };

  let measurementsList' = measurementsList;

  measurementsList' |> E.L.length > 0
    ? <FC.PageCard.Body>
        {Table.fromColumns(
           all,
           measurementsList' |> Array.of_list,
           ~bottomSubRowFn,
           (),
         )}
      </FC.PageCard.Body>
    : <NothingToShow />;
};