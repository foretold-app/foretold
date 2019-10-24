[@bs.config {jsx: 3}];

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

let withPreferenceMutation = innerComponentFn =>
  <EditPreferenceMutation> ...innerComponentFn </EditPreferenceMutation>;
