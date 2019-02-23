open Utility;
open Graph_T.T;

module Query = {
  type condition =
    | IS
    | IS_NOT;

  type t = {
    edge,
    id: string,
    q: condition,
  };

  let run = (q: t, f: fact) => {
    let equality = q.q == IS ? isEqual(q.id) : isNotEqual(q.id);
    switch (q.edge) {
    | SUBJECT => equality(f.subjectId)
    | PROPERTY => equality(f.propertyId)
    | VALUE =>
      switch (f.value.valueType) {
      | ThingId(id) => equality(id)
      | _ => q.q != IS
      }
    };
  };

  let qOr = (qs: list(t), f: fact) => List.exists(q => run(q, f), qs);
  let qAnd = (qs: list(t), f: fact) => List.for_all(q => run(q, f), qs);

  let from_json = i => {
    open Json.Decode;
    let id = i |> field("id", string);
    let _q = i |> field("q", string);
    let _edge = i |> field("edge", string);
    {
      id,
      q:
        switch (_q) {
        | "IS_NOT" => IS_NOT
        | _ => IS
        },
      edge:
        switch (_edge) {
        | "VALUE" => VALUE
        | "PROPERTY" => PROPERTY
        | _ => SUBJECT
        },
    };
  };
};