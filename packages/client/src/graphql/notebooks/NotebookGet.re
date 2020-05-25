module Query = [%graphql
  {|
      query notebook ($id: NotebookId!) {
          notebook (id: $id) {
             id
             name
             body
             ownerId
             channelId
             createdAt
             updatedAt
             isBookmarked
             bookmarksCount
             channel {
               id
               name @bsDecoder(fn: "E.J.toString")
               isArchived
               isPublic
             }
             owner {
               id
               name
               user {
                 id
                 name
                 description
                 agentId
                 picture
               }
               bot {
                 id
                 name
                 description
                 picture
                 competitorType
                 user {
                     id
                     name
                     description
                     picture
                     agentId
                 }
               }
             }
             permissions {
               mutations {
                 allow
               }
             }
           }
      }
    |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let component = (~id: string, innerFn) => {
  let id = id |> E.J.fromString;
  let query = Query.make(~id, ());
  <QueryComponent variables=query##variables fetchPolicy="no-cache">
    ...{({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> Rationale.Result.fmap(e =>
           e##notebook
           |> Rationale.Option.fmap(Primary.Notebook.convertJsObject)
         )
      |> E.R.fmap(innerFn)
      |> E.R.id
    }
  </QueryComponent>;
};