let isFullId = str => str |> String.get(_, 0) == "@".[0];

let toFullId = (baseId, resourceId, localId) =>
  switch (baseId, resourceId, localId) {
  | ("", _, _) => None
  | (a, "", b) => Some("@" ++ a ++ "/" ++ b)
  | (a, b, c) => Some("@" ++ a ++ "/" ++ b ++ "/" ++ c)
  };