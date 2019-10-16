open Style.Grid;

/**
 * Using <Components /> style in React.js is preferable then
 * functions. Try first of all to create new component.
 **/
module FloatPoint = {
  let component = ReasonReact.statelessComponent("FloatPoint");
  let make = (~value: float, _children) => {
    ...component,
    render: _self => {
      let mainText =
        Css.style([
          Css.fontSize(`em(1.1)),
          Css.color(FC__Settings.darkLink),
        ]);

      <Div flexDirection=`column>
        <Div flex={`num(1.)}>
          <div className=mainText>
            {value |> E.Float.toFixed |> Utils.ste}
          </div>
        </Div>
      </Div>;
    },
  };
};

module FloatCdf = {
  let component = ReasonReact.statelessComponent("FloatCdf");
  let make =
      (
        ~value: MeasurementValue.FloatCdf.t,
        ~competitorType: Types.competitorType,
        ~valueText: option(string)=None,
        ~width=150,
        ~height=30,
        _children,
      ) => {
    ...component,
    render: _self => {
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
    },
  };
};

module Binary = {
  let component = ReasonReact.statelessComponent("Binary");
  let make = (~value: bool, _children) => {
    ...component,
    render: _self => {
      let result =
        Css.style([Css.fontSize(`em(1.15)), Css.fontWeight(`bold)]);

      <Div flexDirection=`column>
        <Div flex={`num(1.)}>
          <div className=result>
            {(value ? "True" : "False") |> Utils.ste}
          </div>
        </Div>
      </Div>;
    },
  };
};

module UnresolvableResolution = {
  let component = ReasonReact.statelessComponent("UnresolvableResolution");
  let make = _children => {
    ...component,
    render: _self => {
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
    },
  };
};

module Percentage = {
  let component = ReasonReact.statelessComponent("Percentage");
  let make = (~value: float, _children) => {
    ...component,
    render: _self => {
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
    },
  };
};

module AggregationResolution = {
  let component = ReasonReact.statelessComponent("AggregationResolution");
  let make = (~measurable: Types.measurable, _children) => {
    ...component,
    render: _self => {
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
    },
  };
};