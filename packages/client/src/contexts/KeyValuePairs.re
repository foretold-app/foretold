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

let make = (key, value) => {key, value};