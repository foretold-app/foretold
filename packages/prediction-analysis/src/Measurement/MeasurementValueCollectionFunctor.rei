module Make:
  (
    C: {
      type t('a);
      let map: (t('a), 'a => 'b) => t('b);
      let toMeasurement: t(MeasurementValue.t) => MeasurementValue.t;
    },
  ) =>
   {
    type l('a) = C.t('a);
    type ls('a) = array(C.t('a));
    type t = C.t(MeasurementValue.t);
    type ts = array(t);
    let lengthOfIntendedType: (MeasurementValueWrapper.Name.t, ts) => int;

    module Unwrapped: {
      type t =
        MeasurementValueWrapper.withInsides(
          array(C.t(Cdf.t)),
          array(C.t(float)),
          array(C.t(bool)),
          array(C.t(Percentage.t)),
          array(C.t(MeasurementValue.UnresolvableResolution.t)),
          array(C.t(MeasurementValue.Comment.t)),
        );
      let fromT:
        (MeasurementValueWrapper.Name.t, ts) => Belt.Result.t(t, string);
    };
  };