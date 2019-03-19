type creator = {
  id: string,
  name: option(string),
};

type series = {
  id: string,
  name: option(string),
  description: option(string),
  creator: option(creator),
};

let toSeries = (m: series): DataModel.Series.t =>
  DataModel.Series.make(~id=m.id, ~name=m.name, ());

module Query = [%graphql
  {|
      query getSeries ($id: String!) {
          series: series(id: $id) @bsRecord{
           id
           name
           description
           creator @bsRecord {
             id
             name
           }
          }
      }
    |}
];
module QueryComponent = ReasonApollo.CreateQuery(Query);

let component = (~id, innerFn) => {
  open Rationale.Result.Infix;
  let query = Query.make(~id, ());
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    |> E.R.fmap(e => e##series)
    |> E.R.fmap(innerFn)
    |> E.R.id
  )
  |> E.React.el;
};