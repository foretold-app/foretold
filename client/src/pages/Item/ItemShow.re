open Utils;
open Rationale.Result.Infix;
open Rationale;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("ItemShow");

let findName = (graph, propertyId) =>
  graph
  |> Graph_T.F.factList
  |> Graph_Fact_Filters.withSubject(propertyId)
  |> Graph_Fact_Filters.withProperty("@base/properties/p-name")
  |> Rationale.RList.head
  |> Option.bind(_, (k: Graph_T.T.fact) =>
       switch (k.value.valueType) {
       | String(s) => Some(s)
       | ThingId(s) => Some(s)
       | _ => None
       }
     );

let make = (~id: string, _children) => {
  ...component,
  render: _ => {
    let graph = Data.make;
    let names =
      graph
      |> Graph_T.F.factList
      |> Graph_Fact_Filters.withSubject(id)
      |> List.map((f: Graph_T.T.fact) => f);
    <div>
      <h2> {id |> ste} </h2>
      {
        names
        |> Array.of_list
        |> Array.map((r: Graph_T.T.fact) =>
             <div>
               <h3>
                 {
                   findName(graph, r.propertyId)
                   |> Option.default("no-name")
                   |> ste
                 }
               </h3>
               Graph_T.T.(
                 switch (r.value.valueType) {
                 | String(s) => s |> ste
                 | ThingId(s) =>
                   <a href={Urls.mapLinkToUrl(ItemShow(s))}> {s |> ste} </a>
                 | _ => "no-name" |> ste
                 }
               )
             </div>
           )
        |> ReasonReact.array
      }
    </div>;
  },
};