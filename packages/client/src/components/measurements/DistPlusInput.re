let xyToDist =
    (xyShape: ForetoldBsDistplus.DistTypes.xyShape)
    : ForetoldComponents.Types.Dist.t => {
  let xs = xyShape.xs;
  let ys = xyShape.ys;
  {xs, ys};
};

//
let run = (guesstimatorString: string) => {
  let distPlusIngredients =
    ForetoldBsDistplus.RenderTypes.DistPlusRenderer.Ingredients.make(
      ~guesstimatorString,
      (),
    );
  let inputs =
    ForetoldBsDistplus.RenderTypes.DistPlusRenderer.make(
      ~distPlusIngredients,
      ~recommendedLength=1000,
      (),
    );
  let response = ForetoldBsDistplus.DistPlusRenderer.run(inputs);
  let distPlus = response.distPlus;

  let asFloat =
    switch (
      distPlus
      |> E.O.fmap(ForetoldBsDistplus.Distributions.DistPlus.T.toShape)
    ) {
    | Some(Discrete({xs: [|_|], ys: [|_|]} as foo)) =>
      Some(xyToDist(foo))
    | _ => None
    };

  switch (asFloat) {
  | Some(f) => `Float(f)
  | _ =>
    let integral =
      distPlus
      |> E.O.fmap((r: ForetoldBsDistplus.DistTypes.distPlus) =>
           r.integralCache
         );
    Js.log4(inputs, response, distPlus, integral);
    let dist =
      integral
      |> E.O.fmap((continuous: ForetoldBsDistplus.DistTypes.continuousShape) =>
           continuous.xyShape
         );
    dist
    |> E.O.fmap(xyToDist)
    |> E.O.fmap(r => `Dist(r))
    |> E.O.default(`None);
  };
};