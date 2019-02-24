open Utils;
open Rationale.Result.Infix;
open Rationale;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("ItemShow");

let make = (~id: string, _children) => {
  ...component,
  render: _ => {
    let graph = Data.make;
    let names =
      graph
      |> Graph_T.F.factList
      |> Graph_Fact_Filters.withSubject(id)
      |> List.map((f: Graph_T.T.fact) => f);
    <UseRouterForLinks>
      <div>
        <h2> {id |> ste} </h2>
        {
          names
          |> Array.of_list
          |> Array.map((r: Graph_T.T.fact) =>
               <div>
                 <h3>
                   {
                     graph
                     |> Graph_T.F.factList
                     |> Graph_Fact_Filters.withSubject(r.propertyId)
                     |> Graph_Fact_Filters.withProperty(
                          "@base/properties/p-name",
                        )
                     |> Array.of_list
                     |> Array.map(k =>
                          Graph_T.T.(
                            switch (k.value.valueType) {
                            | String(l) => l |> ste
                            | _ => "no-name" |> ste
                            }
                          )
                        )
                     |> ReasonReact.array
                   }
                 </h3>
                 Graph_T.T.(
                   switch (r.value.valueType) {
                   | String(s) => s |> ste
                   | ThingId(s) => <a href={"/items/" ++ s}> {s |> ste} </a>
                   | _ => "no-name" |> ste
                   }
                 )
               </div>
             )
          |> ReasonReact.array
        }
      </div>
    </UseRouterForLinks>;
  },
};