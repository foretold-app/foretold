type error = string;

type token = string;

type t = {token};

let set = (t: t) => {
  open Dom.Storage;
  localStorage |> setItem("token", t.token);
  ();
};

let destroy = () => {
  open Dom.Storage;
  localStorage |> removeItem("token");
  ();
};

let exists = (t: option(t)) => E.O.isSome(t);

let make_from_storage = () => {
  open Dom.Storage;
  let get = e => localStorage |> getItem(e);
  switch (get("token")) {
  | Some(a) => Some({token: a})
  | _ => None
  };
};

let make = token => {token: token};