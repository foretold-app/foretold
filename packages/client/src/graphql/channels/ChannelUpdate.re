[@bs.config {jsx: 3}];

module Query = [%graphql
  {|
            mutation channelUpdate($id: String!, $input: ChannelInput!) {
                channelUpdate(id: $id, input: $input) {
                 id
                }
            }
    |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate =
    (
      mutation: Mutation.apolloMutation,
      id,
      name,
      description,
      isPublic,
      isArchived,
    ) => {
  let m =
    Query.make(
      ~id,
      ~input={
        "name": name |> E.J.fromString,
        "description": description |> E.J.fromString |> E.O.some,
        "isPublic": isPublic,
        "isArchived": isArchived,
      },
      (),
    );

  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|"getChannels", "user", "channel"|],
    (),
  )
  |> ignore;
};