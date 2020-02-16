let makeGraph = (g: option(Js.Json.t)) =>
  g
  |> E.O.default(Js.Json.array([||]))
  |> (e => BsKen.Interface.Graph.fromJson(e));

let itemUrl = entityId => Routing.Url.toString(EntityShow(entityId));

type g = BsKen.Graph_T.T.t;

let withDefault = (g: option(g)) => g |> E.O.default(makeGraph(None));

let graphFromContext = () => {
  let context = React.useContext(Providers.app);
  context.globalSetting |> E.O.bind(_, r => r.entityGraph) |> withDefault;
};
module FactValue = {
  type value = BsKen.Graph_T.T.value;

  let toString = (value: value) =>
    switch (value.valueType) {
    | String(s) => Some(s)
    | ThingId(s) => Some(s)
    | _ => None
    };
};

module Fact = {
  type fact = BsKen.Graph_T.T.fact;

  let valueAsString = (fact: fact) => fact.value |> FactValue.toString;
};

module SubjectPropertyPair = {
  let allFacts = (g: g, propertyId, subjectId) =>
    g
    |> BsKen.Graph_T.F.factList
    |> BsKen.Graph_Fact_Filters.withSubject(subjectId)
    |> BsKen.Graph_Fact_Filters.withProperty(propertyId);

  let firstFact = (g: g, propertyId, subjectId) =>
    allFacts(g, propertyId, subjectId) |> E.L.head;

  let firstFactValueAsString = (g: g, propertyId, subjectId) =>
    firstFact(g, propertyId, subjectId) |> E.O.bind(_, Fact.valueAsString);
};

module Subject = {
  type id = string;

  let url = (id: id) => Routing.Url.toString(EntityShow(id));

  let facts = (g: g, id: id) =>
    g |> BsKen.Graph_T.F.factList |> BsKen.Graph_Fact_Filters.withSubject(id);

  let name = (g: g, id: id) =>
    SubjectPropertyPair.firstFactValueAsString(
      g,
      "@base/properties/p-name",
      id,
    );

  let instanceOf = (g: g, id: id) =>
    SubjectPropertyPair.firstFactValueAsString(
      g,
      "@base/properties/p-instance-of",
      id,
    );
};

module Thing = {
  type thing = BsKen.Graph_T.Thing.t;
  let all = g => BsKen.Graph_T.F.thingArray(g);
  let id = BsKen.Graph_T.Thing.id;
  let hasName = (g, i) => i |> id |> Subject.name(g) |> E.O.isSome;
  let withNames = g => g |> all |> E.A.filter(hasName(g));
};