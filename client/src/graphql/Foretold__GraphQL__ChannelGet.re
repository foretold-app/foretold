open Utils;

module Query = [%graphql
  {|
    query getChannel($id: String!) {
      channel:
      channel(id: $id){
        id
        name
        description
        isArchived
        isPublic
      }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let toChannel = m: Context.Primary.Channel.t =>
  Context.Primary.Channel.make(
    ~id=m##id,
    ~name=m##name,
    ~description=m##description,
    ~isArchived=m##isArchived,
    ~isPublic=m##isPublic,
    (),
  );

let component = (~id, fn) => {
  let query = Query.make(~id, ());
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    |> E.R.bind(_, e =>
         switch (e##channel |> E.O.fmap(toChannel)) {
         | Some(r) => Ok(r)
         | None => Error("Channel Not Found" |> ste)
         }
       )
    |> E.R.fmap(fn)
    |> E.R.id
  )
  |> E.React.el;
};

type ttt = E.HtppResponse.t(Context.Primary.Channel.t);

let component2 = (~id, innerFn) => {
  let query = Query.make(~id, ());
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> E.HtppResponse.fromApollo
    |> E.HtppResponse.fmap(e => e##channel |> E.O.fmap(toChannel))
    |> E.HtppResponse.optionalToMissing
    |> innerFn
  )
  |> E.React.el;
};