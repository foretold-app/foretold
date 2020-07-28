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
    (
      switch (response.shapeRenderOutputs.symbolic) {
      | Some(Ok({graph: `Simple(`Float(r))})) => Some(r)
      | _ => None
      }
    )
    |> E.O.fmap(r => {
         let wrapped: ForetoldComponents.Types.Dist.t = {
           xs: [|r|],
           ys: [|1.0|],
         };
         wrapped;
       });

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
