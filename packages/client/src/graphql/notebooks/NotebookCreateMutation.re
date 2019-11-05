[@bs.config {jsx: 3}];

module Query = [%graphql
  {|
    mutation notebookCreate($input: NotebookCreateInput!) {
        notebookCreate(input: $input) {
         id
        }
    }
  |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate = (mutation: Mutation.apolloMutation, channelId, name, body) => {
  let channelId = channelId |> E.J.fromString;
  let name = name |> E.J.fromString;
  let body = body |> E.J.fromString;

  let m =
    Query.make(
      ~input={"channelId": channelId, "name": name, "body": body},
      (),
    );

  mutation(~variables=m##variables, ~refetchQueries=[|"notebooks"|], ())
  |> ignore;
};

let withMutation = innerComponentFn =>
  <Mutation onError={e => Js.log2("Graphql Error:", e)}>
    ...innerComponentFn
  </Mutation>;
