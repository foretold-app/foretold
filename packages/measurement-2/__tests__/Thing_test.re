open Jest;
open Expect;

let agentPredictions = [|5, 6, 7|];
let marketPredictions = [|1, 2, 3, 7, 8|];

open PointsAtTimes;

let foo =
  StartAtDistribution.make(
    ~pointXY=[|(5, 5), (10, 10), (20, 20)|],
    ~finalX=30,
  );

let testt = (name, x, value) =>
  test(name, () =>
    expect(StartAtDistribution.xPointToPointXY(x, foo)) |> toEqual(value)
  );

describe("Perimeter", () =>
  describe("xPointToPoint", () => {
    testt("with a very low value", 4, None);
    testt("with a match value", 5, Some((5, 5)));
    testt("with an in-between value", 12, Some((10, 10)));
    testt("when at max", 20, Some((20, 20)));
    testt("when at finalX", 30, Some((20, 20)));
    testt("when above finalX", 35, None);
  })
);