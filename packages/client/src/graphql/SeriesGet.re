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

let toAgent = (m: creator): Types.agent =>
  Primary.Agent.make(~id=m.id, ~name=m.name, ());

let toSeries = (m: series): Types.series =>
  Primary.Series.make(
    ~id=m.id,
    ~name=m.name,
    ~creator=m.creator |> E.O.fmap(toAgent),
    (),
  );

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
  let query = Query.make(~id, ());
  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(e => e##series |> E.O.fmap(toSeries))
      |> E.R.fmap(innerFn)
      |> E.R.id
    }
  </QueryComponent>;
};