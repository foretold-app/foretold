type t = Types.Distribution.t;
type ts = Types.Distribution.ts;

module T: {
  let hasLength: t => bool;
  let empty: t;
  let findX: (float, t) => float;
  let findY: (float, t) => float;
  let make: (array(float), array(float)) => t;
  let toPdf: t => t;
  let integral: t => float;
  let fmap: (float => float, t) => t;
};

module Ts: {
  let minX: (float, ts) => float;
  let maxX: (float, ts) => float;
};