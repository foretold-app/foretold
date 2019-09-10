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
      channelId: string,
    ) => {
  let mutate = {
    let input =
      showDescriptionDate == "TRUE"
        ? {
          "name": name,
          "labelCustom": Some(labelCustom),
          "labelProperty": Some(labelProperty),
          "expectedResolutionDate":
            expectedResolutionDate |> Js.Json.string |> E.O.some,
          "resolutionEndpoint": resolutionEndpoint |> E.O.some,
          "labelSubject": labelSubject |> E.O.some,
          "labelOnDate": labelOnDate |> Js.Json.string |> E.O.some,
          "valueType": `FLOAT,
          "channelId": channelId,
          "min": min == "" ? None : Some(min |> float_of_string),
          "max": max == "" ? None : Some(max |> float_of_string),
        }
        : {
          "name": name,
          "labelCustom": Some(labelCustom),
          "labelProperty": Some(labelProperty),
          "expectedResolutionDate":
            expectedResolutionDate |> Js.Json.string |> E.O.some,
          "resolutionEndpoint": resolutionEndpoint |> E.O.some,
          "labelSubject": labelSubject |> E.O.some,
          "labelOnDate": None,
          "valueType": valueType |> Primary.Measurable.valueTypeToEnum,
          "channelId": channelId,
          "min": min == "" ? None : Some(min |> float_of_string),
          "max": max == "" ? None : Some(max |> float_of_string),
        };

    Query.make(~input, ());
  };

  mutation(
    ~variables=mutate##variables,
    ~refetchQueries=[|"getMeasurables"|],
    (),
  )
  |> ignore;
};