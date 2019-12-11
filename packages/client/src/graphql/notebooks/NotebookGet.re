module Query = [%graphql
  {|
      query getNotebook ($id: NotebookId!) {
          notebook(id: $id) {
             id
             name
             body
             ownerId
             channelId
             createdAt
             updatedAt
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
           }
      }
    |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let component = (~id: string, innerFn) => {
  let id = id |> E.J.fromString;
  let query = Query.make(~id, ());
  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> Rationale.Result.fmap(e =>
           e##notebook
           |> Rationale.Option.fmap(Primary.Notebook.convertJsObject)
         )
      |> Rationale.Result.fmap(innerFn)
      |> E.R.id
    }
  </QueryComponent>;
};