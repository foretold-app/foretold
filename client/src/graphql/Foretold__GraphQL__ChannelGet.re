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

let toChannel = m: DataModel.Channel.t =>
  DataModel.Channel.make(
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