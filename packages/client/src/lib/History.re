type state = Js.nullable({. "miss": bool});
type event = {. "state": state};
// 1
[@bs.module "./history.js"]
external onReplaceState: (event => unit) => unit = "onReplaceState";
[@bs.module "./history.js"]
external onPushState: (event => unit) => unit = "onPushState";
[@bs.module "./history.js"]
external onPopState: (event => unit) => unit = "onPopState";
// 2
[@bs.module "./history.js"]
external pushState: (state, string, string) => unit = "pushState";
[@bs.module "./history.js"]
external replaceState: (state, string, string) => unit = "replaceState";

// 3
type state2 =
  Js.nullable({
    .
    "itemType": Js.nullable(string),
    "itemIndex": Js.nullable(string),
    "directionType": Js.nullable(string),
    "direction": Js.nullable(string),
  });
type event2 = {. "state": state2};
[@bs.module "./history.js"]
external pushState2: (state2, string, string) => unit = "pushState";
[@bs.module "./history.js"]
external replaceState2: (state2, string, string) => unit = "replaceState";
[@bs.module "./history.js"]
external getHistoryState: unit => state2 = "getHistoryState";
[@bs.module "./history.js"]
external onReplaceState2: (event2 => unit) => unit = "onReplaceState";
[@bs.module "./history.js"]
external onPushState2: (event2 => unit) => unit = "onPushState";
[@bs.module "./history.js"]
external onPopState2: (event2 => unit) => unit = "onPopState";

// 4
let miss = event =>
  event##state
  |> Js.Nullable.toOption
  |> E.O.fmap(state => state##miss)
  |> E.O.default(false);

let append = url => {
  //  let state = Js.Nullable.fromOption(Some({"miss": true}));
  //  pushState(state, "", url);
};