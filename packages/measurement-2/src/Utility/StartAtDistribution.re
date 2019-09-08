type t('a, 'b) = {
  finalX: 'a,
  pointXY: array(('a, 'b)),
};

//The Xs should be unique and should be sorted.
let _compareXs = ((x1, _), (x2, _)) => compare(x1, x2);
let _sortXUniq = t =>
  t.pointXY |> Array.to_list |> List.sort_uniq(_compareXs) |> Array.of_list;

let make = (~pointXY, ~finalX) => {
  finalX,
  pointXY: _sortXUniq({finalX, pointXY}),
};

let map = (fn, t: t('a, 'b)) => {
  finalX: t.finalX,
  pointXY: t.pointXY |> Array.map(((x, y)) => (x, fn(y))),
};

let xs = t => t.pointXY |> Array.map(((x, _)) => x);
let ys = t => t.pointXY |> Array.map(((_, y)) => y);
let minX = t => t |> xs |> SortedArray.min;

let transposeResult =
    (t: t('a, Belt.Result.t('b, 'c))): Belt.Result.t(t('a, 'b), 'c) => {
  switch (Belt.Array.getBy(ys(t), Belt.Result.isError)) {
  | Some(Error(r)) => Error(r)
  | _ => Belt.Result.Ok(t |> map(Belt.Result.getExn))
  };
};

// Finds relevant (x,y) point at or before a specific x point.
let xPointToRelevantPointXY = (xPoint, t: t('a, 'b)) =>
  xPoint > t.finalX
    ? None
    : t.pointXY
      |> Belt.Array.reverse
      |> Belt.Array.getBy(_, ((x, _)) => x <= xPoint);

let xPointToRelevantPointX = (xPoint, t: t('a, 'b)) =>
  xPointToRelevantPointXY(xPoint, t) |> Belt.Option.map(_, ((x, _)) => x);

let xPointToRelevantPointY = (xPoint, t: t('a, 'b)) =>
  xPointToRelevantPointXY(xPoint, t) |> Belt.Option.map(_, ((_, y)) => y);

let toSortedArrayTillFinalX = (t: t('a, 'b)) => {
  switch (xPointToRelevantPointXY(t.finalX, t)) {
  | Some((x, _)) when x == t.finalX => Some(t.pointXY)
  | Some((x, y)) => Some(SortedArray.concat(t.pointXY, [|(t.finalX, y)|]))
  | _ => None
  };
};

let integral = (t: t(float, float)) => {
  t
  |> toSortedArrayTillFinalX
  |> Rationale.Option.bind(_, Utility.Array.toRanges)
  |> Rationale.Option.fmap(inRanges =>
       inRanges
       |> Belt.Array.map(_, (((lastX, lastY), (nextX, _))) =>
            (nextX -. lastX) *. lastY
          )
       |> Belt.Array.reduce(_, 0., (a, b) => a +. b)
     );
};

// If ether is empty, should return empty list.
let relevantXs = (a: t('a, 'b), b: t('a, 'c)) => {
  let firstCommonMin =
    switch (minX(a), minX(b)) {
    | (Some(a), Some(b)) => Some(min(a, b))
    | _ => None
    };
  switch (firstCommonMin) {
  | Some(firstCommonMin) =>
    SortedArray.concat(xs(a), xs(b))
    |> SortedArray.filterOutLessThan(firstCommonMin)
  | None => [||]
  };
};

let product = (t1: t('a, 'b), t2: t('a, 'c)): t('a, ('b, 'c)) => {
  let xs = relevantXs(t1, t2);
  {
    finalX: min(t1.finalX, t2.finalX),
    pointXY:
      xs
      |> Array.map(x =>
           switch (
             xPointToRelevantPointY(x, t1),
             xPointToRelevantPointY(x, t2),
           ) {
           | (Some(y1), Some(y2)) => Some((x, (y1, y2)))
           | _ => None
           }
         )
      |> Utility.Array.concatSome,
  };
};