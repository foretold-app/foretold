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

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate =
    (mutation: Mutation.apolloMutation, email: string, channelId: string) => {
  let mutate =
    Query.make(~input={"email": email, "channelId": channelId}, ());
  mutation(~variables=mutate##variables, ~refetchQueries=[||], ()) |> ignore;
};

let component = innerComponentFn =>
  <Mutation onError={e => Js.log2("Graphql Error:", e)}>
    ...innerComponentFn
  </Mutation>;
