type time = int;
type ts = array(time);

// let agentPredictions = [5, 6, 7];
// let marketPredictions = [1, 2, 3, 7, 8];
// let perimeter = [5, 8];
// let points = [5, 6, 7, 8];

module SortedArray = {
  type t = array(int);
  let sort = Belt.SortArray.stableSortBy(_, compare);

  //   let sort = (t: array(int)) => Belt.SortArray.Int(t);
  let make = (t: array(int)) =>
    Belt.SortArray.isSorted(t, compare)
      ? t : Belt.SortArray.stableSortBy(t, compare);

  let concat = (a: t, b: t): t => Belt.Array.concat(a, b) |> sort |> make;

  let filterOutLessThan = (lowestAllowedValue: int, t: t): t =>
    Belt.Array.keep(t, x => x >= lowestAllowedValue);

  let filterOutGreaterThan = (highestAllowedValue: int, t: t): t =>
    Belt.Array.keep(t, x => x >= highestAllowedValue);

  let firstBeforeValue = (v: int, t: t) => {
    let firstIndex = Belt.Array.getIndexBy(t, r => r >= v);
    let previousIndex =
      switch (firstIndex) {
      | None => None
      | Some(0) => None
      | Some(n) => Some(n)
      };
    let previousValue =
      previousIndex |> Belt.Option.flatMap(_, Belt.Array.get(t));
    previousValue;
  };

  let firstAtOrBeforeValue = (v: int, t: t) => 3;

  let filterLessThanInclusivePlusOne = (highestAllowedValue: int, t: t): t => {
    let atAndAfter = filterOutLessThan(highestAllowedValue, t);
    let previousValue = firstBeforeValue(highestAllowedValue, t);
    switch (previousValue) {
    | None => atAndAfter
    | Some(e) => concat([|e|], atAndAfter) |> sort |> make
    };
  };
};

let minMax = (ts: ts, ts: ts) => {
  (3, 5);
};

type pair = {
  agent: time,
  market: time,
};

let pairs = [
  {agent: 5, market: 3},
  {agent: 6, market: 3},
  {agent: 7, market: 7},
  {agent: 7, market: 8},
];