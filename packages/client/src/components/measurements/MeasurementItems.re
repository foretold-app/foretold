open Style.Grid;

module FloatPoint = {
  [@react.component]
  let make = (~value: float) => {
    let mainText =
      Css.style([
        Css.fontSize(`em(1.1)),
        Css.color(ForetoldComponents.Settings.darkLink),
      ]);

    <Div flexDirection=`column>
      <Div flex={`num(1.)}>
        <div className=mainText>
          {value |> E.Float.with2DigitsPrecision |> Utils.ste}
        </div>
      </Div>
    </Div>;
  };
};

module FloatCdf = {
  [@react.component]
  let make =
      (
        ~value: MeasurementValue.FloatCdf.t,
        ~competitorType: Types.competitorType,
        ~valueText=None,
        ~width=150,
        ~height=35,
        ~xMin: option(float),
        ~xMax: option(float),
      ) => {
    let color =
      competitorType == `AGGREGATION ? `hex("b1b9c6") : `hex("487192");

    let dist =
      value
      |> MeasurementValue.toPdf
      |> MeasurementValue.FloatCdf.toJs
      |> (
        data => <SmallCdfChart data width height color minX=xMin maxX=xMax />
      );

    let stat =
      value
      |> MeasurementValue.FloatCdf.toJs
      |> ForetoldComponents.Base.Types.Dist.fromJson
      |> (cdf => <ForetoldComponents.CdfChart__StatSummary cdf />);

    <Div flexDirection=`row>
      <Div flex={`num(1.)}> stat </Div>
      <Div flex={`num(3.)}> dist </Div>
    </Div>;
  };

  // We basically never change these, so they should generally stay constant.
  let make =
    React.memoCustomCompareProps(make, (next, current) =>
      next##value == current##value
    );
};

module Binary = {
  [@react.component]
  let make = (~value: bool) => {
    let result =
      Css.style([Css.fontSize(`em(1.15)), Css.fontWeight(`bold)]);

    <Div flexDirection=`column>
      <Div flex={`num(1.)}>
        <div className=result> {(value ? "True" : "False") |> Utils.ste} </div>
      </Div>
    </Div>;
  };
};

module UnresolvableResolution = {
  [@react.component]
  let make = () => {
    let secondaryText =
      Css.style([
        Css.fontSize(`em(0.9)),
        Css.color(ForetoldComponents.Settings.accentBlue),
      ]);

    <Div flexDirection=`column>
      <Div flex={`num(1.)}>
        <div className=secondaryText>
          {"Closed Without Answer" |> Utils.ste}
        </div>
      </Div>
    </Div>;
  };
};

module Percentage = {
  [@react.component]
  let make = (~value: float) => {
    let mainText =
      Css.style([
        Css.fontSize(`em(1.1)),
        Css.color(ForetoldComponents.Settings.darkLink),
      ]);

    let secondaryText =
      Css.style([
        Css.fontSize(`em(0.9)),
        Css.color(ForetoldComponents.Settings.accentBlue),
      ]);

    <Div flexDirection=`column>
      <Div flex={`num(1.)}>
        <div className=mainText>
          <ForetoldComponents.PercentageShower precision=8 percentage=value />
        </div>
      </Div>
      <Div flex={`num(1.)}>
        <div className=secondaryText> {"Mean" |> Utils.ste} </div>
      </Div>
    </Div>;
  };
};

module MeasurementShow = {
  [@react.component]
  let make =
      (
        ~measurement: option(Types.measurement),
        ~xMin: option(float),
        ~xMax: option(float),
      ) => {
    switch (measurement) {
    | Some(measurement) =>
      switch (measurement.value) {
      | Ok(`FloatPoint(r)) => <FloatPoint value=r />
      | Ok(`FloatCdf(r)) =>
        <FloatCdf
          xMin
          xMax
          value=r
          valueText={measurement.valueText}
          competitorType={measurement.competitorType}
        />
      | Ok(`UnresolvableResolution(_)) => <UnresolvableResolution />
      | Ok(`Percentage(r)) => <Percentage value=r />
      | Ok(`Binary(r)) => <Binary value=r />
      | _ => <Null />
      }
    | _ => <Null />
    };
  };
};

module ResolutionOrRecentAggregation = {
  [@react.component]
  let make =
      (
        ~measurable: Types.measurable,
        ~xMin: option(float),
        ~xMax: option(float),
      ) => {
    <MeasurementShow
      xMin
      xMax
      measurement={E.O.firstSome(
        measurable.outcome,
        measurable.previousAggregate,
      )}
    />;
  };
};

module AgentPrediction = {
  [@react.component]
  let make =
      (
        ~measurable: Types.measurable,
        ~xMin: option(float),
        ~xMax: option(float),
      ) => {
    <MeasurementShow
      xMin
      xMax
      measurement={
        switch (measurable.recentMeasurement) {
        | Some(measurement) =>
          switch (Primary.Measurement.valueSuggestsJudgement(measurement)) {
          | Some(true) => None
          | _ => Some(measurement)
          }
        | _ => None
        }
      }
    />;
  };
};

module MeasurementDebugForm = {
  let formatTime = r =>
    r
    |> E.O.fmap((m: MomentRe.Moment.t) => E.M.goFormat_standard(m))
    |> E.O.default("")
    |> Utils.ste;

  [@react.component]
  let make = (~measurement: Types.measurement) => {
    <Antd.Form layout=`vertical labelAlign=`left>
      <Antd.Form.Item label={"ID" |> Utils.ste}>
        <Antd.Input value={measurement.id} readOnly=true />
      </Antd.Form.Item>
      <Antd.Form.Item label={"Competitor Type" |> Utils.ste}>
        <Antd.Input
          value={Primary.CompetitorType.toString(measurement.competitorType)}
          readOnly=true
        />
      </Antd.Form.Item>
      {E.React2.showIf(
         measurement.competitorType === `AGGREGATION,
         {<Antd.Form.Item label={"Tagged ID" |> Utils.ste}>
            <Antd.Input
              value={measurement.taggedMeasurementId |> E.O.default("")}
              readOnly=true
            />
          </Antd.Form.Item>},
       )}
      <Antd.Form.Item label={"Created At" |> Utils.ste}>
        {formatTime(measurement.createdAt)}
      </Antd.Form.Item>
      <Antd.Form.Item label={"Relevant At" |> Utils.ste}>
        {formatTime(measurement.relevantAt)}
      </Antd.Form.Item>
      <Antd.Form.Item label={"Value" |> Utils.ste}>
        <Antd.Input.TextArea
          value={
            switch (measurement.value) {
            | Ok(r) => MeasurementValue.encode(r) |> Js.Json.stringify
            | _ => ""
            }
          }
          readonly=true
          cols=3
        />
      </Antd.Form.Item>
      <Antd.Form.Item label={"Text Input" |> Utils.ste}>
        <Antd.Input
          value={
            switch (measurement.valueText) {
            | Some(r) => r
            | _ => ""
            }
          }
          readOnly=true
        />
      </Antd.Form.Item>
      <Antd.Form.Item label={"Description" |> Utils.ste}>
        <Antd.Input.TextArea
          value={measurement.description |> E.O.default("")}
          readonly=true
          cols=5
        />
      </Antd.Form.Item>
    </Antd.Form>;
  };
};

module Info = {
  let overlayClassName = Css.style([Css.width(`em(40.))]);

  module ById = {
    [@react.component]
    let make = (~measurementId) => {
      <Antd_Popover
        overlayClassName
        placement=`left
        content={MeasurementGet.component(~id=measurementId, measurement =>
          measurement
          |> E.O.fmap(measurement => <MeasurementDebugForm measurement />)
          |> E.O.default("No Data" |> Utils.ste)
        )}>
        <div className=Style.iconGray> <Icon icon="COPY" /> </div>
      </Antd_Popover>;
    };
  };

  [@react.component]
  let make = (~measurement: Types.measurement) => {
    <Antd_Popover
      overlayClassName
      placement=`left
      content={<MeasurementDebugForm measurement />}>
      <div className=Style.iconGray> <Icon icon="COPY" /> </div>
    </Antd_Popover>;
  };
};