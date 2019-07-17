type t = {
  key: string,
  value: string,
};

type ts = array(t);

let fromSearchParam = (r: string) =>
  r
  |> Js.String.split("&")
  |> E.A.fmap(r => {
       let param = Js.String.split("=", r) |> E.A.to_list;
       switch (param) {
       | [key, value] => Some({key, value})
       | _ => None
       };
     })
  |> E.A.O.concatSome;

let get = (key, ts: ts) =>
  ts
  |> E.A.filter(r => r.key == key)
  |> E.A.get(_, 0)
  |> E.O.fmap(r => r.value);

let toUrlParams = (ts: ts) =>
  ts
  |> E.A.fmap(r => r.key ++ "=" ++ r.value)
  |> Js.Array.joinWith("&")
  |> (r => "?" ++ r);

let toUrlHash = (ts: ts) =>
  ts |> E.A.fmap(r => r.key ++ "=" ++ r.value) |> Js.Array.joinWith("&");

let clearHash = (url: ReasonReact.Router.url, removeItem: string) => {
  let path = url.path |> Array.of_list |> Js.Array.joinWith("/");
  let hashes =
    url.hash
    |> fromSearchParam
    |> Js.Array.filter((item: t) => item.key != removeItem)
    |> toUrlHash;
  let hashes' = hashes == "" ? "" : "#" ++ hashes;
  let search = url.search == "" ? "" : "?" ++ url.search;
  path ++ search ++ hashes';
};

let make = (key, value) => {key, value};