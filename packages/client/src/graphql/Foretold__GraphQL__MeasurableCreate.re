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

// @todo: unduplicate
type values = {
  name: string,
  labelCustom: string,
  labelSubject: string,
  labelOnDate: string,
  labelProperty: string,
  expectedResolutionDate: string,
  resolutionEndpoint: string,
  showDescriptionDate: string,
  showDescriptionProperty: string,
};

let mutate =
    (mutation: Mutation.apolloMutation, values: values, channelId: string) => {
  let preparedQuery =
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
    ~variables=preparedQuery##variables,
    ~refetchQueries=[|"getMeasurables"|], // @todo
    (),
  )
  |> ignore;
};

let withMutation = innerComponentFn =>
  Mutation.make(
    ~onError=e => Js.log2("Graphql Error:", e),
    innerComponentFn,
  );