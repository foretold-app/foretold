type t('a) = array('a);

let uniq = (a: array('a)) =>
  a |> Array.to_list |> List.sort_uniq(compare) |> Array.of_list;

let fromArray = (a: array('a)) =>
  a |> Belt.SortArray.stableSortBy(_, compare) |> uniq;

let make = (t: array('a)) =>
  Belt.SortArray.isSorted(t, compare) ? Some(t) : None;

let concat = (a: t('a), b: t('a)): t('a) =>
  Belt.Array.concat(a, b) |> fromArray;

let filterOutLessThan = (lowestAllowedValue: 'a, t: t('a)): t('a) =>
  Belt.Array.keep(t, x => x >= lowestAllowedValue);

let filterOutGreaterThan = (highestAllowedValue: 'a, t: t('a)): t('a) =>
  Belt.Array.keep(t, x => x >= highestAllowedValue);

let min = Belt.Array.get(_, 0);
let max = t => Belt.Array.get(t, Belt.Array.length(t) - 1);

// Currenty not used
let firstBeforeValue = (v: 'a, t: t('a)) => {
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

// Currenty not used
let firstAtOrBeforeValue = (v: 'a, t: t('a)) => {
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

// Currenty not used
let filterOutLessThanButKeepFirstBefore =
    (lowestAllowedValue: 'a, t: t('a)): t('a) => {
  let atAndAfter = filterOutLessThan(lowestAllowedValue, t);
  let previousValue = firstBeforeValue(lowestAllowedValue, t);
  switch (previousValue) {
  | None => atAndAfter
  | Some(e) => concat([|e|], atAndAfter) |> fromArray
  };
};