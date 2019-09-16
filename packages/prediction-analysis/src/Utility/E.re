type time = int;

module Result = {
  let listFlatten = (results: list(Belt.Result.t('a, 'b))) =>
    List.for_all(Belt.Result.isOk, results)
      ? Belt.Result.Ok(List.map(Belt.Result.getExn, results))
      : Error(
          results
          |> List.map(Rationale.Result.getError)
          |> List.filter(Rationale.Option.isSome)
          |> List.map(Belt.Option.getExn),
        );

  let arrayFlatten = (results: array(Belt.Result.t('a, 'b))) =>
    Belt.Array.some(results, Belt.Result.isError)
      ? Belt.Result.Error(
          results
          |> Array.map(Rationale.Result.getError)
          |> Belt.Array.keep(_, Rationale.Option.isSome)
          |> Array.map(Belt.Option.getExn),
        )
      : Ok(Array.map(Belt.Result.getExn, results));
};

module Array = {
  let concatSome = (optionals: array(option('a))): array('a) =>
    optionals
    |> Js.Array.filter(Rationale.Option.isSome)
    |> Js.Array.map(
         Rationale.Option.toExn("Warning: This should not have happened"),
       );

  let defaultEmpty = (o: option(array('a))): array('a) =>
    switch (o) {
    | Some(o) => o
    | None => [||]
    };

  let toRanges = (a: array('a)) =>
    switch (a |> Belt.Array.length) {
    | 0
    | 1 => Belt.Result.Error("Must be at least 2 elements")
    | n =>
      Belt.Array.makeBy(n - 1, r => r)
      |> Belt.Array.map(_, index =>
           (
             Belt.Array.getUnsafe(a, index),
             Belt.Array.getUnsafe(a, index + 1),
           )
         )
      |> Rationale.Result.return
    };
  let fold_left = Array.fold_left;
  let fold_right = Array.fold_right;
};

module FloatArray = {
  let min = r => r |> Array.fold_left((a, b) => a < b ? a : b, max_float);
  let max = r => r |> Array.fold_left((a, b) => a > b ? a : b, min_float);
};