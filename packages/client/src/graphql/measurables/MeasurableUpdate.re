module Query = [%graphql
  {|
             mutation measurableUpdate($id: String!, $input: MeasurableUpdateInput!) {
                 measurableUpdate(id: $id, input: $input) {
                   id
                 }
             }
     |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate =
    (
      mutation: Mutation.apolloMutation,
      id: string,
      name: string,
      labelCustom: string,
      expectedResolutionDate: string,
      resolutionEndpoint: string,
      labelSubject: string,
      labelOnDate: string,
      showDescriptionDate: string,
      labelProperty: string,
      valueType: string,
      min: string,
      max: string,
    ) => {
  let date = showDescriptionDate == "TRUE" ? labelOnDate : "";

  let m =
    Query.make(
      ~id,
      ~input={
        "name": name,
        "labelCustom": labelCustom |> Rationale.Option.some,
        "labelProperty": labelProperty |> Rationale.Option.some,
        "labelOnDate": date |> Js.Json.string |> Rationale.Option.some,
        "expectedResolutionDate":
          expectedResolutionDate |> Js.Json.string |> Rationale.Option.some,
        "resolutionEndpoint": resolutionEndpoint |> Rationale.Option.some,
        "labelSubject": labelSubject |> Rationale.Option.some,
        "valueType": valueType |> Primary.Measurable.valueTypeToEnum,
        "min":
          min != ""
            ? min |> Js.Float.fromString |> Rationale.Option.some : None,
        "max":
          max != ""
            ? max |> Js.Float.fromString |> Rationale.Option.some : None,
      },
      (),
    );

  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|"getAgent", "getMeasurable", "getMeasurements"|],
    (),
  )
  |> ignore;
};