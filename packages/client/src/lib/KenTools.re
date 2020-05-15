type g = BsKen.Graph_T.T.t;

module Graph = {
  let fromJson = (g: option(Js.Json.t)) =>
    g
    |> E.O.default(Js.Json.array([||]))
    |> (e => BsKen.Interface.Graph.fromJson(e));

  let withDefault = (g: option(g)) => g |> E.O.default(fromJson(None));

  let fromContext = () => {
    let context = React.useContext(Providers.app);
    context.globalSetting |> E.O.bind(_, r => r.entityGraph) |> withDefault;
  };
};

module FactValue = {
  type value = BsKen.Graph_T.T.value;

  let toString = (value: value) =>
    switch (value.valueType) {
    | String(s) => Some(s)
    | ThingId(s) => Some(s)
    | _ => None
    };

  let thingId = (value: value) =>
    switch (value.valueType) {
    | String(_) => None
    | ThingId(s) => Some(s)
    | _ => None
    };
};

module Fact = {
  type fact = BsKen.Graph_T.T.fact;
  let all = g => g |> BsKen.Graph_T.F.factList;

  let value = (fact: fact) => fact.value;
};

module SubjectPropertyPair = {
  let allFacts = (g: g, propertyId, subjectId) =>
    g
    |> BsKen.Graph_T.F.factList
    |> BsKen.Graph_Fact_Filters.withSubject(subjectId)
    |> BsKen.Graph_Fact_Filters.withProperty(propertyId);

  let firstFact = (g: g, propertyId, subjectId) =>
    allFacts(g, propertyId, subjectId) |> E.L.head;

  let firstFactValue = (g: g, propertyId, subjectId) =>
    firstFact(g, propertyId, subjectId) |> E.O.fmap(Fact.value);

  let firstFactValueAsString = (g: g, propertyId, subjectId): option(string) =>
    firstFactValue(g, propertyId, subjectId)
    |> E.O.bind(_, FactValue.toString);
};

module ValuePropertyPair = {
  let allFacts = (g: g, propertyId, valueId) =>
    g
    |> BsKen.Graph_T.F.factList
    |> BsKen.Graph_Fact_Filters.withValue(valueId)
    |> BsKen.Graph_Fact_Filters.withProperty(propertyId);

  let firstFact = (g: g, propertyId, subjectId) =>
    allFacts(g, propertyId, subjectId) |> E.L.head;

  let firstFactValue = (g: g, propertyId, subjectId) =>
    firstFact(g, propertyId, subjectId) |> E.O.fmap(Fact.value);

  let firstFactValueAsString = (g: g, propertyId, subjectId): option(string) =>
    firstFactValue(g, propertyId, subjectId)
    |> E.O.bind(_, FactValue.toString);
};

module Subject = {
  type id = string;

  let url = (id: id) => Routing.Url.toString(EntityShow(id));

  let facts = (g: g, id: id) =>
    g |> BsKen.Graph_T.F.factList |> BsKen.Graph_Fact_Filters.withSubject(id);

  let isValueToFacts = (g: g, id: id) =>
    g |> BsKen.Graph_T.F.factList |> BsKen.Graph_Fact_Filters.withValue(id);

  let name = (g: g, id: id) =>
    SubjectPropertyPair.firstFactValue(g, "@base/properties/p-name", id);

  let instanceOf = (g: g, id: id) =>
    SubjectPropertyPair.firstFactValue(
      g,
      "@base/properties/p-instance-of",
      id,
    );

  let nameWithLink = (g: g, id: id) =>
    <Link linkType={Internal(EntityShow(id))}>
      {name(g, id)
       |> E.O.bind(_, FactValue.toString)
       |> E.O.default("")
       |> Utils.ste}
    </Link>;
};

module Thing = {
  type t = BsKen.Graph_T.T.thing;
  type thing = BsKen.Graph_T.Thing.t;
  let all = g => BsKen.Graph_T.F.thingArray(g);
  let id = BsKen.Graph_T.Thing.id;
  let hasName = (g, i) => i |> id |> Subject.name(g) |> E.O.isSome;
  let withNames = g => g |> all |> E.A.filter(hasName(g));
};

let toString = (g, attribute) =>
  attribute |> Subject.name(g) |> E.O.bind(_, FactValue.toString);