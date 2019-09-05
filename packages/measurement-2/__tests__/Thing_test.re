open Jest;
open Expect;

let agentPredictions = [|5, 6, 7|];
let marketPredictions = [|1, 2, 3, 7, 8|];

describe("Perimeter", () =>
  test("works", () =>
    expect(PointsAtTimes.perimeter(agentPredictions, marketPredictions))
    |> toBe([3])
  )
);