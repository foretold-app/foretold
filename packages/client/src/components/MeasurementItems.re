[@bs.config {jsx: 3}];

open Style.Grid;

/**
 * Using <Components /> style in React.js is preferable then
 * functions. Try first of all to create new component.
 **/
module FloatPoint = {
  [@react.component]
  let make = (~value: float) => {
    let mainText =
      Css.style([
        Css.fontSize(`em(1.1)),
        Css.color(FC__Settings.darkLink),
      ]);

    <Div flexDirection=`column>
      <Div flex={`num(1.)}>
        <div className=mainText> {value |> E.Float.toFixed |> Utils.ste} </div>
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
        ~valueText: option(string)=None,
        ~width=150,
        ~height=30,
      ) => {
    let color =
      competitorType == `AGGREGATION ? `hex("b1b9c6") : `hex("487192");

    let dist =
      value
      |> MeasurementValue.toPdf
      |> MeasurementValue.FloatCdf.toJs
      |> (data => <SmallCdfChart data width height color />);

    let stat =
      value
      |> MeasurementValue.FloatCdf.toJs
      |> FC.Base.Types.Dist.fromJson
      |> (cdf => <FC__CdfChart__StatSummary cdf />);

    <Div flexDirection=`row>
      <Div flex={`num(1.)}> stat </Div>
      <Div flex={`num(1.)}> dist </Div>
    </Div>;
  };
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
        Css.color(FC__Settings.accentBlue),
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
        Css.color(FC__Settings.darkLink),
      ]);

    let secondaryText =
      Css.style([
        Css.fontSize(`em(0.9)),
        Css.color(FC__Settings.accentBlue),
      ]);

    <Div flexDirection=`column>
      <Div flex={`num(1.)}>
        <div className=mainText>
          <FC__PercentageShower precision=8 percentage=value />
        </div>
      </Div>
      <Div flex={`num(1.)}>
        <div className=secondaryText> {"Mean" |> Utils.ste} </div>
      </Div>
    </Div>;
  };
};

module AggregationResolution = {
  [@react.component]
  let make = (~measurable: Types.measurable) => {
    switch (measurable.previousAggregate, measurable.outcome) {
    | (_, Some(measurement)) =>
      switch (measurement.value) {
      | Ok(`FloatPoint(r)) => <FloatPoint value=r />
      | Ok(`FloatCdf(r)) =>
        <FloatCdf
          value=r
          valueText={measurement.valueText}
          competitorType={measurement.competitorType}
        />
      | Ok(`Binary(r)) => <Binary value=r />
      | Ok(`UnresolvableResolution(r)) => <UnresolvableResolution />
      | _ => ReasonReact.null
      }

    | (Some(measurement), None) =>
      switch (measurement.value) {
      | Ok(`FloatCdf(r)) =>
        <FloatCdf
          value=r
          valueText={measurement.valueText}
          competitorType={measurement.competitorType}
        />
      | Ok(`Percentage(r)) => <Percentage value=r />
      | _ => ReasonReact.null
      }

    | _ => ReasonReact.null
    };
  };
};
