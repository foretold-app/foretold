module Query = [%graphql
  {|
    mutation preferenceUpdate(
        $id: String!
        $input: PreferenceUpdateInput!
    ) {
        preferenceUpdate(
            id: $id
            input: $input
        ) {
            id
        }
    }
 |}
];

module EditPreferenceMutation = ReasonApollo.CreateMutation(Query);

let mutate =
    (
      mutation: EditPreferenceMutation.apolloMutation,
      stopAllEmails: bool,
      enableExperimentalFeatures: bool,
      id: string,
    ) => {
  let mutate =
    Query.make(
      ~id,
      ~input={
        "stopAllEmails": Some(stopAllEmails),
        "enableExperimentalFeatures": Some(enableExperimentalFeatures),
      },
      (),
    );
  mutation(~variables=mutate##variables, ~refetchQueries=[||], ()) |> ignore;
};

let component = (auth0Id, innerComponentFn: 'a => ReasonReact.reactElement) => {
  let query = UserGet.Query.make(~auth0Id, ());
  <UserGet.QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(innerComponentFn)
      |> E.R.id
    }
  </UserGet.QueryComponent>;
};

let withPreferenceMutation = innerComponentFn =>
  <EditPreferenceMutation onError={e => Js.log2("Graphql Error:", e)}>
    ...innerComponentFn
  </EditPreferenceMutation>;
