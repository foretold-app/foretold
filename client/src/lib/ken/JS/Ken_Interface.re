open Rationale.Function.Infix;

module Graph = {
  [@genType]
  let fromJson =
    CompressedImporter__FromJson.run
    ||> CompressedImporter__ToSimpleFactList.run
    ||> SimpleFactList_ToGraph.run
    ||> Graph_Dirs.makeDirs;

  [@genType]
  let things = Graph_T.F.thingArray;

  [@genType]
  let facts = Graph_T.F.factArray;

  [@genType]
  let factList = Graph_T.F.factList;

  [@genType]
  let directoryArray = Graph_T.F.directories ||> Array.of_list;

  [@genType]
  let childDirectories = (g, s) =>
    Graph_T.F.childDirectories(g, s) |> Array.of_list;

  [@genType]
  let rootDirectories = Graph_T.F.rootDirectories ||> Array.of_list;

  [@genType]
  let findThing = Graph_T.F.findThing;

  [@genType]
  let findThingFromFact = Graph_Graph.findThingFromFact;

  [@genType]
  let to_json = Graph_Graph.to_json;

  module EdgeTypes = {
    [@genType]
    let property = Graph_T.T.PROPERTY;

    [@genType]
    let subject = Graph_T.T.SUBJECT;
  };
};

module Thing = {
  [@genType]
  let to_json = Graph_T.Thing.to_json;
};

module Directory = {
  [@genType]
  let parent = Graph_T.Directory.parent;
};

module Fact = {
  [@genType]
  let to_json = Graph_Fact.to_json;
  [@genType]
  let value = Graph_Fact.value;
};

module Filter = {
  [@genType]
  let query = Graph_Fact_Filters.query;
};

module Query = {
  [@genType]
  let from_json = Graph_Fact_Query.Query.from_json;
};

module Value = {
  [@genType]
  let to_json = Graph_Value.to_json;
};

[@genType]
let list_to_array = Array.of_list;