open Utils;

type metaTypes = [ | `Date | `Entity | `String];

/* The ID of the entity */
type entity = string;

type human = {name: string};

type measurable = {id: string};

type action('a) = {
  name: string,
  metaTypes: array((string, metaTypes)),
};

type metaType = {
  t: metaTypes,
  id: string,
  name: string,
  description: option(string),
  default: string,
};

module Id =
  Belt.Id.MakeComparable({
    type t = string;
    let cmp: (string, string) => int = Pervasives.compare;
  });

type map = Belt.Map.t(Id.t, string, Id.identity);

let toMap = e => e |> Js.Dict.entries |> Belt.Map.fromArray(~id=(module Id));

type measurableType = {
  id: string,
  name: string,
  description: string,
  schema: list(metaType),
  form: ReasonReact.reactElement,
  nameFn: map => string,
  descriptionFn: map => string,
};

let makeMetaType = (~t, ~id, ~name, ~description=None, ~default="", ()) => {
  t,
  id,
  name,
  description,
  default,
};

let makeMeasurableType =
    (
      ~id,
      ~name="",
      ~description="",
      ~schema,
      ~nameFn=(e: list(metaType), a: map) => "",
      ~descriptionFn=(e: list(metaType), a: map) => "",
      ~form=<div />,
      (),
    ) => {
  id,
  name,
  description,
  schema,
  form,
  nameFn: nameFn(schema),
  descriptionFn: descriptionFn(schema),
};