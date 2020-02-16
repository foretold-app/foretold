// open BsKen;
 // module type KenModule = {
 //   type t = Graph_T.T.thing;
 //   let graph: Graph_T.T.t;
 //   let itemUrl: string => string;
 //   let findName: string => option(string);
 //   let names: string => list(Graph_T.T.fact);
 //   let findInstanceOfName: string => option(Graph_T.T.thingIdString);
 //   let things: array(t);
 //   let getName: t => string;
 //   let getInstanceOfName: t => string;
 //   let withNames: Js.Array.t(t) => Js.Array.t(t);
 //   let dataSource: Js.Array.t(t);
 // };
 // module Functor =
 //        (Config: {let globalSetting: option(Types.globalSetting);})
 //        : KenModule => {
 //   type t = Graph_T.T.thing;
 //   let graph =
 //     Config.globalSetting
 //     |> E.O.fmap((globalSetting: Types.globalSetting) =>
 //          globalSetting.entityGraph |> E.O.default(Js.Json.null)
 //        )
 //     |> E.O.default(Js.Json.array([||]))
 //     |> (e => Interface.Graph.fromJson(e));
 //   // TODO: This interface command isn't obvious.
 //   let itemUrl = entityId => Routing.Url.toString(EntityShow(entityId));
 //   module FactValue = {
 //     type value = Graph_T.T.value;
 //     let toString = (value: value) =>
 //       switch (value.valueType) {
 //       | String(s) => Some(s)
 //       | ThingId(s) => Some(s)
 //       | _ => None
 //       };
 //   };
 //   module Fact = {
 //     type fact = Graph_T.T.fact;
 //     let valueAsString = (fact: fact) => fact.value |> FactValue.toString;
 //   };
 //   module SubjectPropertyPair = {
 //     let allFacts = (propertyId, subjectId) =>
 //       graph
 //       |> Graph_T.F.factList
 //       |> Graph_Fact_Filters.withSubject(subjectId)
 //       |> Graph_Fact_Filters.withProperty(propertyId);
 //     let firstFact = (propertyId, subjectId) =>
 //       allFacts(propertyId, subjectId) |> E.L.head;
 //     let firstFactValueAsString = (propertyId, subjectId) =>
 //       firstFact(propertyId, subjectId) |> E.O.bind(_, Fact.valueAsString);
 //   };
 //   module Subject = {
 //     type id = string;
 //     let url = (id: id) => Routing.Url.toString(EntityShow(id));
 //     let facts = (id: id) =>
 //       graph |> Graph_T.F.factList |> Graph_Fact_Filters.withSubject(id);
 //     let name =
 //       SubjectPropertyPair.firstFactValueAsString("@base/properties/p-name");
 //     let instanceOf =
 //       SubjectPropertyPair.firstFactValueAsString(
 //         "@base/properties/p-instance-of",
 //       );
 //   };
 //   let findName = Subject.name;
 //   let findInstanceOfName = Subject.instanceOf;
 //   let names = subjectId =>
 //     graph
 //     |> Graph_T.F.factList
 //     |> Graph_Fact_Filters.withSubject(subjectId)
 //     |> E.L.fmap((f: Graph_T.T.fact) => f);
 //   let things = graph |> Graph_T.F.thingArray;
 //   let getName = t => t |> Graph_T.Thing.id |> findName |> E.O.default("");
 //   let getInstanceOfName = t =>
 //     t |> Graph_T.Thing.id |> findInstanceOfName |> E.O.default("");
 //   let hasName = t => getName(t) != "";
 //   let withNames = E.A.filter(hasName);
 //   let dataSource = things |> withNames;
 /* }*/