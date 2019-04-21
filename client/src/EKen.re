module Thing = {
  type t = Graph_T.T.thing;
  let getName = (t: t) =>
    t |> Graph_T.Thing.id |> C.Ken.findName |> E.O.default("");
  let hasName = (t: t) => getName(t) != "";
};

module Things = {
  type t = array(Thing.t);
  let getAll = C.Ken.things;
  let withNames = E.A.filter(Thing.hasName);
};