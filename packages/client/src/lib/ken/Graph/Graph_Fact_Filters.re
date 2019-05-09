open Rationale;
open Graph_T.T;
open Graph_Fact_Query;

type t = list(fact);

let query = (q: Query.t, t) => t |> List.filter(Query.run(q));

let find = (id, t) => t |> RList.find((e: fact) => e.thingIdString == id);
let filter = List.filter;

let withQuery = query => filter(Query.run(query));
let withEdge = (edge, id) => withQuery({edge, q: Query.IS, id});
let withoutEdge = (edge, id) => withQuery({edge, q: Query.IS_NOT, id});
let withSubject = id => withQuery({edge: SUBJECT, q: Query.IS, id});
let withoutSubject = id => withQuery({edge: SUBJECT, q: Query.IS_NOT, id});
let withProperty = id => withQuery({edge: PROPERTY, q: Query.IS, id});
let withoutProperty = id => withQuery({edge: PROPERTY, q: Query.IS_NOT, id});
let withValue = id => withQuery({edge: VALUE, q: Query.IS, id});
let withoutValue = id => withQuery({edge: VALUE, q: Query.IS_NOT, id});

let withIdAsAnyEdge = id =>
  filter(
    Query.qOr([
      {edge: SUBJECT, q: Query.IS, id},
      {edge: PROPERTY, q: Query.IS, id},
      {edge: VALUE, q: Query.IS, id},
    ]),
  );

let withIdAsNoEdge = id =>
  filter(
    Query.qAnd([
      {edge: SUBJECT, q: Query.IS_NOT, id},
      {edge: PROPERTY, q: Query.IS_NOT, id},
      {edge: VALUE, q: Query.IS_NOT, id},
    ]),
  );