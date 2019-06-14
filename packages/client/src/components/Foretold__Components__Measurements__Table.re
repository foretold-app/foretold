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
        [color(`rgba((80, 80, 80, 0.78))), marginBottom(`px(0))],
      ),
    ]);

  let date = style([color(`rgba((35, 35, 35, 0.34)))]);

  let agentStyle =
    style([color(`rgb((102, 121, 134))), fontSize(`em(1.1))]);
};

module Helpers = {
  let smallDistribution = (m: measurement, g: (float, float)) =>
    switch (m.value) {
    | Belt.Result.Ok(`FloatCdf(r)) =>
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
                m.competitorType == `AGGREGATION
                  ? `hex("b1b9c6") : `hex("487192")
              }
            />,
          )
      );
    | Belt.Result.Ok(`FloatPoint(r)) =>
      Some(
        <div className=Styles.middle>
          {r |> E.Float.with3DigitsPrecision |> ste}
        </div>,
      )
    | _ => None
    };

  let statSummary = (m: measurement) =>
    switch (m.value) {
    | Belt.Result.Ok(`FloatCdf(r)) =>
      r
      |> MeasurementValue.toChunks(~bucketSize=3)
      |> MeasurementValue.FloatCdf.toJs
      |> FC.Base.Types.Dist.fromJson
      |> (cdf => Some(<FC__CdfChart__StatSummary cdf />))
    | Belt.Result.Ok(`FloatPoint(r)) =>
      Some(r |> E.Float.with3DigitsPrecision |> ste)
    | _ => None
    };

  let description = (~m: measurement) =>
    switch (m.description) {
    | None
    | Some("") => None
    | Some(description) =>
      Some(
        <div className=Styles.descriptionStyle>
          {description |> ste |> E.React.inP}
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
    | Belt.Result.Ok(`FloatCdf(r)) => Some(r)
    | _ => None
    };

  let bounds = (m: Js_array.t(measurement)) => {
    let itemBounds =
      m
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

  let floatCdf = e =>
    switch (e##value) {
    | Belt.Result.Ok(`FloatCdf(r)) => Some(r)
    | _ => None
    };

  let toChartMeasurement = (m: measurement) =>
    switch (m.value) {
    | Belt.Result.Ok(`FloatCdf(r)) =>
      switch (
        E.FloatCdf.firstAboveValue(0.05, r),
        E.FloatCdf.firstAboveValue(0.95, r),
      ) {
      | (Some(low), Some(high)) => Some((low, high))
      | _ => None
      }
    | _ => None
    };

  let measurerLink = (~m: measurement) => {
    let agent = m.agent;
    let aLink =
      switch (agent, agent |> E.O.bind(_, Context.Primary.Agent.name)) {
      | (Some(agent), Some(name)) =>
        Some(
          <Foretold__Components__Link
            linkType={
              Internal(Agent({agentId: agent.id, subPage: AgentShow}))
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
    let isJudge = Context.Primary.Measurement.isJudgement(m);

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

let getItems = (ms: list(measurement), ~makeItem) => {
  let _bounds = Helpers.bounds(ms |> E.A.of_list);
  ms
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

let make = (ms: list(measurement)) => {
  let makeItem = (m, _bounds) => {
    <>
      <FC.Table.Cell flex=2 className=primaryCellStyle>
        {Helpers.smallDistribution(m, _bounds) |> E.O.React.defaultNull}
      </FC.Table.Cell>
      <FC.Table.Cell flex=1 className=primaryCellStyle>
        {Helpers.statSummary(m) |> E.O.React.defaultNull}
      </FC.Table.Cell>
      <FC.Table.Cell flex=1 className=primaryCellStyle>
        {Helpers.measurerLink(~m)}
      </FC.Table.Cell>
      <FC.Table.Cell flex=1 className=primaryCellStyle>
        {Helpers.relevantAt(~m) |> E.O.React.defaultNull}
      </FC.Table.Cell>
    </>;
  };
  let items = ms |> getItems(~makeItem);
  E.React.showIf(
    ms |> E.L.length > 0,
    <>
      <FC.Table.HeaderRow>
        <FC.Table.Cell flex=2>
          {"Prediction Distribution" |> ste}
        </FC.Table.Cell>
        <FC.Table.Cell flex=1> {"Prediction Value" |> ste} </FC.Table.Cell>
        <FC.Table.Cell flex=1> {"Agent" |> ste} </FC.Table.Cell>
        <FC.Table.Cell flex=1> {"Time" |> ste} </FC.Table.Cell>
      </FC.Table.HeaderRow>
      items
    </>,
  );
};

let make2 = (ms: list(measurement)) => {
  let makeItem = (m: measurement, _bounds) => {
    <>
      <FC.Table.Cell flex=1 className=primaryCellStyle>
        {switch (m.measurable) {
         | None => "" |> ste
         | Some(measurable) => Items.link(~m=measurable)
         }}
      </FC.Table.Cell>
      <FC.Table.Cell flex=2 className=primaryCellStyle>
        {Helpers.smallDistribution(m, _bounds) |> E.O.React.defaultNull}
      </FC.Table.Cell>
      <FC.Table.Cell flex=1 className=primaryCellStyle>
        {Helpers.statSummary(m) |> E.O.React.defaultNull}
      </FC.Table.Cell>
      <FC.Table.Cell flex=1 className=primaryCellStyle>
        {Helpers.measurerLink(~m)}
      </FC.Table.Cell>
      <FC.Table.Cell flex=1 className=primaryCellStyle>
        {Helpers.relevantAt(~m) |> E.O.React.defaultNull}
      </FC.Table.Cell>
    </>;
  };
  let items = ms |> getItems(~makeItem);
  E.React.showIf(
    ms |> E.L.length > 0,
    <>
      <FC.Table.HeaderRow>
        <FC.Table.Cell flex=1> {"Measurable" |> ste} </FC.Table.Cell>
        <FC.Table.Cell flex=2>
          {"Prediction Distribution" |> ste}
        </FC.Table.Cell>
        <FC.Table.Cell flex=1> {"Prediction Value" |> ste} </FC.Table.Cell>
        <FC.Table.Cell flex=1> {"Agent" |> ste} </FC.Table.Cell>
        <FC.Table.Cell flex=1> {"Time" |> ste} </FC.Table.Cell>
      </FC.Table.HeaderRow>
      items
    </>,
  );
};