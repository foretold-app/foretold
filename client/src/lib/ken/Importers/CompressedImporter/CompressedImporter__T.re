type value =
  | String(string)
  | Array(array(string));

/* Note that the thing ID is not in the fact. */
/* Change import area. */
[@bs.deriving jsConverter]
type fact = {
  id: option(string),
  property: string,
  isInversed: bool,
  value,
};

[@bs.deriving jsConverter]
type thing = {
  id: string,
  facts: array(fact),
};

type package = {
  things: array(thing),
  baseId: string,
  resourceId: string,
  aliases: Js.Dict.t(string),
};

type graph = array(package);