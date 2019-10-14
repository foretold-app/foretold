module Query = [%graphql
  {|
    mutation userUpdate(
        $id: String!
        $input: UserUpdateInput!
    ) {
        userUpdate(
            id: $id
            input: $input
        ) {
            id
        }
    }
 |}
];

module EditUserMutation = ReasonApollo.CreateMutation(Query);

let mutate =
    (
      mutation: EditUserMutation.apolloMutation,
      name: string,
      email: string,
      picture: string,
      description: string,
      id: string,
    ) => {
  let email' = email === "" ? None : Some(email);
  let picture' = picture === "" ? None : Some(picture);
  let description' = picture === "" ? None : Some(description);

  let mutate =
    Query.make(
      ~id,
      ~input={
        "name": Some(name),
        "email": email',
        "picture": picture',
        "description": description',
        "auth0AccessToken": None,
      },
      (),
    );

  mutation(~variables=mutate##variables, ~refetchQueries=[|"user"|], ())
  |> ignore;
};

let withUserMutation = innerComponentFn =>
  <EditUserMutation onError={e => Js.log2("Graphql Error:", e)}>
    ...innerComponentFn
  </EditUserMutation>;
