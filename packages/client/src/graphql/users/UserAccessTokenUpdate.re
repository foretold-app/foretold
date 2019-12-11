module Query = [%graphql
  {|
    mutation userAccessTokenUpdate(
        $id: String!
        $input: UserAccessTokenUpdateInput!
    ) {
        userAccessTokenUpdate(
            id: $id
            input: $input
        ) {
            id
        }
    }
 |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate =
    (mutation: Mutation.apolloMutation, id: string, auth0AccessToken: string) => {
  let mutate =
    Query.make(
      ~id,
      ~input={"auth0AccessToken": Some(auth0AccessToken)},
      (),
    );
  mutation(~variables=mutate##variables, ~refetchQueries=[||], ()) |> ignore;
};