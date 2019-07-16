open Rationale.Function.Infix;
type error = string;

type access_token = string;
type id_token = string;
type expires_at = string;
type token = string;

type t = {
  access_token,
  id_token,
  expires_at,
  token,
};

let jwt = (t: t) => Jwt.make(t.id_token);
let auth0Id = (t: t) => t |> jwt |> E.O.bind(_, Jwt.get_attribute("sub"));

let set = (t: t) => {
  open Dom.Storage;
  localStorage |> setItem("access_token", t.access_token);
  localStorage |> setItem("id_token", t.id_token);
  localStorage |> setItem("expires_at", t.expires_at);
  localStorage |> setItem("token", t.token);
  ();
};

let destroy = () => {
  open Dom.Storage;
  localStorage |> removeItem("access_token");
  localStorage |> removeItem("id_token");
  localStorage |> removeItem("expires_at");
  localStorage |> removeItem("token");
  ();
};

let exists = (t: option(t)) => E.O.isSome(t);

let isObsolete = (t: t) => {
  let exp = t.expires_at;
  let isExpired =
    Int64.of_string ||> Int64.to_float ||> (e => e < E.JsDate.now());
  exp |> isExpired;
};

let make_from_storage = () => {
  open Dom.Storage;
  let get = e => localStorage |> getItem(e);
  switch (
    get("access_token"),
    get("id_token"),
    get("expires_at"),
    get("token"),
  ) {
  | (Some(a), Some(b), Some(c), Some(d)) =>
    Some({access_token: a, id_token: b, expires_at: c, token: d})
  | _ => None
  };
};

let make = (access_token, id_token, expires_at, token) => {
  access_token,
  id_token,
  expires_at,
  token,
};