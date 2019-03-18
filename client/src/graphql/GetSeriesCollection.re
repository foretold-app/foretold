open Rationale;
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
let toSeries = (m: series): DataModel.series =>
  DataModel.toSeries(~id=m.id, ~name=m.name, ());

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
  open Result.Infix;
  let query = Query.make();
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    <$> (e => e##seriesCollection)
    <$> (e => e |> E.JsArray.concatSomes)
    <$> innerFn
    |> E.R.id
  )
  |> E.React.el;
};