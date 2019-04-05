module Query = [%graphql
  {|
            mutation seriesCreate($name: String, $description: String, $channelId: String!, $subjects: [String], $properties: [String], $dates: [Date]) {
                seriesCreate(name: $name, description: $description, channelId: $channelId, subjects: $subjects, properties:$properties, dates:$dates) {
                  name
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
      channelId,
      subjects,
      properties,
      dates,
    ) => {
  let m =
    Query.make(
      ~name,
      ~description,
      ~channelId,
      ~subjects,
      ~properties,
      ~dates,
      (),
    );
  mutation(~variables=m##variables, ~refetchQueries=[|"getChannels"|], ())
  |> ignore;
};

let withMutation = innerComponentFn =>
  Mutation.make(~onError=e => Js.log2("Graphql Error:", e), innerComponentFn)
  |> E.React.el;