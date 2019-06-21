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

let toChannel = (channel): Context.Primary.Channel.t =>
  Context.Primary.Channel.make(
    ~id=channel##id,
    ~name=channel##name,
    ~description=channel##description,
    ~isArchived=channel##isArchived,
    ~isPublic=channel##isPublic,
    ~myRole=Some(channel##myRole),
    ~membershipCount=Some(channel##membershipCount),
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
  |> ReasonReact.element;
};