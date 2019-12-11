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
      query getSeriesCollection($channelId: String) {
          seriesCollection(channelId: $channelId) @bsRecord{
           id
           name
           description
           channel @bsRecord {
             id
             name @bsDecoder(fn: "E.J.toString")
             description @bsDecoder(fn: "E.J.O.toString")
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

let component = (channelId, innerFn) => {
  let query = Query.make(~channelId, ());

  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(e => e##seriesCollection)
      |> E.R.fmap(E.JsArray.concatSomes)
      |> E.R.fmap(innerFn)
      |> E.R.id
    }
  </QueryComponent>;
};

let component2 = (~channelId, innerFn) => {
  let query = Query.make(~channelId, ());
  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> HttpResponse.fromApollo
      |> HttpResponse.fmap(e => e##seriesCollection |> E.JsArray.concatSomes)
      |> innerFn
    }
  </QueryComponent>;
};