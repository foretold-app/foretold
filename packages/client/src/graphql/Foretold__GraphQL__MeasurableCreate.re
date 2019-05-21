module Query = [%graphql
  {|
             mutation measurableCreate($input: MeasurableCreateInput!) {
                 measurableCreate(input: $input) {
                   id
                 }
             }
  |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate =
    (
      mutation: Mutation.apolloMutation,
      values: SignUpForm.values,
      channelId: string,
    ) => {
  let mutate =
    values.showDescriptionDate == "TRUE"
      ? Query.make(
          ~input={
            "name": values.name,
            "labelCustom": Some(values.labelCustom),
            "labelProperty": Some(values.labelProperty),
            "expectedResolutionDate":
              values.expectedResolutionDate |> Js.Json.string |> E.O.some,
            "resolutionEndpoint": values.resolutionEndpoint |> E.O.some,
            "labelSubject": values.labelSubject |> E.O.some,
            "labelOnDate": values.labelOnDate |> Js.Json.string |> E.O.some,
            "valueType": `FLOAT,
            "channelId": channelId,
          },
          (),
        )
      : Query.make(
          ~input={
            "name": values.name,
            "labelCustom": Some(values.labelCustom),
            "labelProperty": Some(values.labelProperty),
            "expectedResolutionDate":
              values.expectedResolutionDate |> Js.Json.string |> E.O.some,
            "resolutionEndpoint": values.resolutionEndpoint |> E.O.some,
            "labelSubject": values.labelSubject |> E.O.some,
            "labelOnDate": None,
            "valueType": `FLOAT,
            "channelId": channelId,
          },
          (),
        );

  mutation(
    ~variables=mutate##variables,
    ~refetchQueries=[|"getMeasurables"|], // @todo
    (),
  )
  |> ignore;
};