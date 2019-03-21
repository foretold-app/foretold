type creator = {
  id: string,
  name: option(string),
};

type series = {
  id: string,
  name: option(string),
  description: option(string),
  channel: string,
  measurableCount: option(int),
  creator: option(creator),
};

type seriesCollection = array(series);

module Query = [%graphql
  {|
      query getSeriesCollection {
          seriesCollection @bsRecord{
           id
           name
           description
           channel
           measurableCount
           creator @bsRecord {
             id
             name
           }
          }
      }
    |}
];
module QueryComponent = ReasonApollo.CreateQuery(Query);

let component = innerFn => {
  open Rationale.Result.Infix;
  let query = Query.make();
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    |> E.R.fmap(e => e##seriesCollection)
    |> E.R.fmap(E.JsArray.concatSomes)
    |> E.R.fmap(innerFn)
    |> E.R.id
  )
  |> E.React.el;
};