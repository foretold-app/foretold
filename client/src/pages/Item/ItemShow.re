open Utils;
open Rationale.Result.Infix;
open Rationale;

let component = ReasonReact.statelessComponent("ItemShow");

let findName = (graph, propertyId) =>
  graph
  |> Graph_T.F.factList
  |> Graph_Fact_Filters.withSubject(propertyId)
  |> Graph_Fact_Filters.withProperty("@base/properties/p-name")
  |> E.L.head
  |> E.O.bind(_, (k: Graph_T.T.fact) =>
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
      |> E.L.fmap((f: Graph_T.T.fact) => f);
    <div>
      <SLayout.Header> {SLayout.Header.textDiv(id)} </SLayout.Header>
      <SLayout.MainSection>
        {
          names
          |> E.A.of_list
          |> E.A.fmap((r: Graph_T.T.fact) =>
               <div>
                 <h3>
                   {
                     findName(graph, r.propertyId)
                     |> E.O.default("no-name")
                     |> ste
                   }
                 </h3>
                 Graph_T.T.(
                   switch (r.value.valueType) {
                   | String(s) => s |> ste
                   | ThingId(s) =>
                     <a href={Urls.mapLinkToUrl(ItemShow(s))}>
                       {s |> ste}
                     </a>
                   | _ => "no-name" |> ste
                   }
                 )
               </div>
             )
          |> ReasonReact.array
        }
      </SLayout.MainSection>
    </div>;
  },
};