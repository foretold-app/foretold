type time = int;

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
};