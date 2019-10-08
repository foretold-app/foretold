module EditPreference = [%graphql
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

module EditPreferenceMutation = ReasonApollo.CreateMutation(EditPreference);

let mutate =
    (
      mutation: EditPreferenceMutation.apolloMutation,
      stopAllEmails: bool,
      enableExperimentalFeatures: bool,
      id: string,
    ) => {
  let mutate =
    EditPreference.make(
      ~id,
      ~input={
        "stopAllEmails": Some(stopAllEmails),
        "enableExperimentalFeatures": Some(enableExperimentalFeatures),
      },
      (),
    );
  mutation(~variables=mutate##variables, ~refetchQueries=[|"user"|], ())
  |> ignore;
};

let withUserQuery =
    (auth0Id, innerComponentFn: 'a => ReasonReact.reactElement) => {
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
  EditPreferenceMutation.make(
    ~onError=e => Js.log2("Graphql Error:", e),
    innerComponentFn,
  )
  |> E.React.el;
