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
};