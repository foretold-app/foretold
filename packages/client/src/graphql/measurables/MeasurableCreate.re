module GraphQL = [%graphql
  {|
             mutation measurableCreate($input: MeasurableCreateInput!) {
                 measurableCreate(input: $input) {
                   id
                 }
             }
     |}
];

module Mutation = ReasonApollo.CreateMutation(GraphQL);

let mutate =
    (
      mutation: Mutation.apolloMutation,
      values: MeasurableForm.MeasurableReForm.values,
      channelId: string,
    ) => {
  let mutate = {
    let input =
      values.showDescriptionDate == "TRUE"
        ? {
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
          "min":
            values.min == "" ? None : Some(values.min |> float_of_string),
          "max":
            values.max == "" ? None : Some(values.max |> float_of_string),
        }
        : {
          "name": values.name,
          "labelCustom": Some(values.labelCustom),
          "labelProperty": Some(values.labelProperty),
          "expectedResolutionDate":
            values.expectedResolutionDate |> Js.Json.string |> E.O.some,
          "resolutionEndpoint": values.resolutionEndpoint |> E.O.some,
          "labelSubject": values.labelSubject |> E.O.some,
          "labelOnDate": None,
          "valueType": values.valueType |> Primary.Measurable.valueTypeToEnum,
          "channelId": channelId,
          "min":
            values.min == "" ? None : Some(values.min |> float_of_string),
          "max":
            values.max == "" ? None : Some(values.max |> float_of_string),
        };

    GraphQL.make(~input, ());
  };

  mutation(
    ~variables=mutate##variables,
    ~refetchQueries=[|"getMeasurables"|],
    (),
  )
  |> ignore;
};