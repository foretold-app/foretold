type time = float;

type t('a) = {
  measurementValue: 'a,
  time,
};

let make = (~time, ~measurementValue, ()) => {measurementValue, time};