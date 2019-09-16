open Jest;
open Expect;

let agentPredictions = [|5, 6, 7|];
let marketPredictions = [|1, 2, 3, 7, 8|];

let distributuionA =
  StartAtDistribution.make(
    ~pointXY=[|(5., 5.), (10., 10.), (20., 20.)|],
    ~finalX=30.,
  );

let distributuionB =
  StartAtDistribution.make(~pointXY=[|(8., 8.), (25., 25.)|], ~finalX=40.);

describe("StartAtDistribution", () => {
  describe("#xPointToRelevantPointXY", () => {
    let genTest = (name, x, value) =>
      test(name, () =>
        expect(
          StartAtDistribution.xPointToRelevantPointXY(x, distributuionA),
        )
        |> toEqual(value)
      );
    genTest("with a very low value", 4., None);
    genTest("with a match value", 5., Some((5., 5.)));
    genTest("with an in-between value", 12., Some((10., 10.)));
    genTest("when at max", 20., Some((20., 20.)));
    genTest("when at finalX", 30., Some((20., 20.)));
    genTest("when above finalX", 35., None);
  });
  test("#map", () =>
    expect(
      distributuionA
      |> StartAtDistribution.map(r => r *. 2.)
      |> (r => r.pointXY),
    )
    |> toEqual([|(5., 10.), (10., 20.), (20., 40.)|])
  );
  test("#integral", () =>
    expect(distributuionA |> StartAtDistribution.integral)
    |> toEqual(Belt.Result.Ok(325.))
  );
  test("#product", () =>
    expect(StartAtDistribution.product(distributuionA, distributuionB))
    |> toEqual(
         StartAtDistribution.make(
           ~finalX=30.0,
           ~pointXY=[|
             (8., (5., 8.)),
             (10., (10., 8.)),
             (20., (20., 8.)),
             (25., (20., 25.)),
           |],
         ),
       )
  );
});