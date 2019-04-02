module Query = [%graphql
  {|
            mutation channelCreate($input: ChannelInput!) {
                channelCreate(input: $input) {
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
    ~refetchQueries=[|"getChannels", "user"|],
    (),
  )
  |> ignore;
};