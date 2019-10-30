[@bs.config {jsx: 3}];

type error = string;

type jwt = string;
type t = jwt;

let set = (t: t) => {
  open Dom.Storage;
  localStorage |> setItem("server_jwt", t);
  ();
};

let destroy = () => {
  open Dom.Storage;
  localStorage |> removeItem("server_jwt");
  ();
};

let exists = (t: option(t)) => E.O.isSome(t);

let make_from_storage = (): option(t) => {
  open Dom.Storage;
  let get = e => localStorage |> getItem(e);
  get("server_jwt");
};
