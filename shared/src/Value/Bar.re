open Rationale;

module type PointValue = {
  type t;
  let equal: (t, t) => bool;
  let isValid: t => bool;
};

type percentile = float;
let allTrue = e => Array.fold_left((x, y) => x && y, true, e);

module MakeByPercentile = (Item: PointValue) => {
  type byPercentile = array((percentile, Item.t));

  let isValid = (byPercentile: byPercentile) => {
    let itemsAreValid =
      Array.map(((_, b)) => Item.isValid(b), byPercentile) |> allTrue;

    let structureIsValid = true;
    itemsAreValid && structureIsValid;
  };
};

module Float = {
  type t = float;
  let equal = (a: t, b: t) => a == b;
  let isValid = e => true;
};

module Percentage = {
  type t = float;
  let equal = (a: t, b: t) => a == b;
  let isValid = e => e >= 0 && e <= 100;
};

module Bar = MakeByPercentile(Float);