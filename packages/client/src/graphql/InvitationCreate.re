[@bs.config {jsx: 3}];

module Query = [%graphql
  {|
    mutation invitationCreate(
        $input: InvitationCreateInput!
    ) {
        invitationCreate(
            input: $input
        ) {
            id
        }
    }
 |}
];