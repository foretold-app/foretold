open Rationale;
type series = {
  id: string,
  name: option(string),
  description: option(string),
};

let toSeries = (m: series): DataModel.series =>
  DataModel.toSeries(~id=m.id, ~name=m.name, ());

module Query = [%graphql
  {|
      query getSeries ($id: String!) {
          series: series(id: $id) @bsRecord{
           id
           name
           description
          }
      }
    |}
];
module QueryComponent = ReasonApollo.CreateQuery(Query);

let component = (~id, innerFn) => {
  open Result.Infix;
  let query = Query.make(~id, ());
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    <$> (e => e##series)
    <$> innerFn
    |> E.R.id
  )
  |> ReasonReact.element;
};