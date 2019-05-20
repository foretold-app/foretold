[@bs.module] external stringFromFile: Js.Json.t = "./Data.json";

let make = Ken_Interface.Graph.fromJson(stringFromFile);