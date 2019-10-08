module Query = [%graphql
  {|
            mutation seriesCreate($input:SeriesCreateInput!) {
                seriesCreate(input: $input) {
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
      ~input={
        "name": Some(name),
        "description": Some(description),
        "channelId": channelId,
        "subjects": Some(subjects),
        "properties": Some(properties),
        "dates": Some(dates),
      },
      (),
    );
  mutation(~variables=m##variables, ~refetchQueries=[|"getChannels"|], ())
  |> ignore;
};

let component = innerComponentFn =>
  <Mutation onError={e => Js.log2("Graphql Error:", e)}>
    ...innerComponentFn
  </Mutation>;
