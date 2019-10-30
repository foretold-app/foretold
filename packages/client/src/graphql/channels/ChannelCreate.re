[@bs.config {jsx: 3}];

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

let mutate =
    (
      mutation: Mutation.apolloMutation,
      name,
      description,
      isPublic,
      isArchived,
    ) => {
  let m =
    Query.make(
      ~input={
        "name": name,
        "description": description,
        "isPublic": isPublic,
        "isArchived": isArchived,
      },
      (),
    );
  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|"getChannels", "user"|],
    (),
  )
  |> ignore;
};
