module Query = [%graphql
  {|
    mutation invitationCreate (
        $input: InvitationCreateInput!
    ) {
        invitationCreate (
            input: $input
        ) {
            id
        }
    }
 |}
];

module Mutation = ReasonApolloHooks.Mutation.Make(Query);