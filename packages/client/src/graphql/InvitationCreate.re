module Query = [%graphql
  {|
    mutation invitationCreate(
        $input: InvitationCreateInput!
    ) {
        invitationCreate(
            input: $input
        )
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

let withMutation = innerComponentFn =>
  Mutation.make(~onError=e => Js.log2("Graphql Error:", e), innerComponentFn)
  |> E.React.el;