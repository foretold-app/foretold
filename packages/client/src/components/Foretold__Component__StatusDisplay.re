open Utils;

let statusShow = (~measurable: Context.Primary.Measurable.t) =>
  switch (
    Context.Primary.Measurable.toStatus(measurable),
    measurable.expectedResolutionDate,
    measurable.stateUpdatedAt,
  ) {
  | (`OPEN, Some(resDate), _) =>
    FC.StateStatus.make(~state=OPEN(resDate), ~fontSize=`em(0.85), ())
  | (`JUDGEMENT_PENDING, Some(resDate), _) =>
    FC.StateStatus.make(~state=PENDING(resDate), ~fontSize=`em(0.85), ())
  | (`JUDGED, _, Some(resolvedAt)) =>
    FC.StateStatus.make(
      ~state=RESOLVED(resolvedAt),
      ~fontSize=`em(0.85),
      (),
    )
  | (_, _, _) => ReasonReact.null
  };
let component = ReasonReact.statelessComponent("MeasurableShow");

let futureTime = 1559005200;

let make = (~measurable: Context.Primary.Measurable.t, _children) => {
  ...component,
  render: _self => statusShow(~measurable),
};