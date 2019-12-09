[@bs.config {jsx: 3}];

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