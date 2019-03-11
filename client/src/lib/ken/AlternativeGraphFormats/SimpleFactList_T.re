type thingId = string;
type valueType = Graph_T.T.valueType;
type value = Graph_T.T.value;

type id = {
  id: string,
  isPublic: bool,
};

[@bs.deriving jsConverter]
type fact = {
  id,
  subjectId: id,
  propertyId: id,
  value,
};

type graph = list(fact);

let showFacts = (g: graph) => g |> E.A.of_list |> E.A.fmap(factToJs);

let combine = (l: list(graph)): graph => l |> E.L.concat;