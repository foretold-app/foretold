type state = Js.nullable({. "miss": bool});

type event = {. "state": state};

[@bs.module "./history.js"]
external onReplaceState: (event => unit) => unit = "onReplaceState";
[@bs.module "./history.js"]
external onPushState: (event => unit) => unit = "onPushState";
[@bs.module "./history.js"]
external onPopState: (event => unit) => unit = "onPopState";

[@bs.module "./history.js"]
external pushState: (state, string, string) => unit = "pushState";
[@bs.module "./history.js"]
external replaceState: (state, string, string) => unit = "replaceState";

let miss = event =>
  event##state
  |> Js.Nullable.toOption
  |> E.O.fmap(state => state##miss)
  |> E.O.default(false);

let append = url => {
  let state = Js.Nullable.fromOption(Some({"miss": true}));
  pushState(state, "", url);
};