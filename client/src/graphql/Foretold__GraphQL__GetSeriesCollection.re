type creator = {
  id: string,
  name: option(string),
};

type channel = {
  id: string,
  name: string,
  description: option(string),
};

type series = {
  id: string,
  name: option(string),
  description: option(string),
  channel: option(channel),
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
           channel:Channel @bsRecord {
             id
             name
             description
           }
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

type ttt = Client.E.HttpResponse.t(Js.Array.t(series));
let component2 = innerFn => {
  open Rationale.Result.Infix;
  let query = Query.make();
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> E.HttpResponse.fromApollo
    |> E.HttpResponse.fmap(e => e##seriesCollection |> E.JsArray.concatSomes)
    |> innerFn
  )
  |> E.React.el;
};