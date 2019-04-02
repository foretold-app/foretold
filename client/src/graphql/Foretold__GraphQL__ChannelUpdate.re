module Query = [%graphql
  {|
            mutation channelUpdate($input: ChannelInput!) {
                channelUpdate(input: $input) {
                 id
                }
            }
    |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate = (mutation: Mutation.apolloMutation, name, description, isPublic) => {
  let m =
    Query.make(
      ~input={"name": name, "description": description, "isPublic": isPublic},
      (),
    );
  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|"getChannels", "user", "channel"|],
    (),
  )
  |> ignore;
};