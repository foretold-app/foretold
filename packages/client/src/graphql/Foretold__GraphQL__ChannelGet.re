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
        membershipCount
        myRole
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
    ~myRole=Some(m##myRole),
    ~membershipCount=Some(m##membershipCount),
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
         | None => Error("Community Not Found" |> ste)
         }
       )
    |> E.R.fmap(fn)
    |> E.R.id
  )
  |> E.React.el;
};

let component2 = (~id, innerFn) => {
  let query = Query.make(~id, ());
  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> E.HttpResponse.fromApollo
    |> E.HttpResponse.fmap(e => e##channel |> E.O.fmap(toChannel))
    |> E.HttpResponse.optionalToMissing
    |> innerFn
  )
  |> E.React.el;
};