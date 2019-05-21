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

type values = {
  name: string,
  labelCustom: string,
  labelSubject: string,
  labelOnDate: Js.Nullable.t(string),
  labelProperty: string,
  expectedResolutionDate: Js.Nullable.t(string),
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
              values.expectedResolutionDate
              |> Js.Nullable.toOption
              |> Utils.resolveOption
              |> Js.Json.string
              |> E.O.some,
            "resolutionEndpoint": values.resolutionEndpoint |> E.O.some,
            "labelSubject": values.labelSubject |> E.O.some,
            "labelOnDate":
              values.labelOnDate
              |> Js.Nullable.toOption
              |> Utils.resolveOption
              |> Js.Json.string
              |> E.O.some,
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
              values.expectedResolutionDate
              |> Js.Nullable.toOption
              |> Utils.resolveOption
              |> Js.Json.string
              |> E.O.some,
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

let withMutation = (~onCompleted, ~innerComponentFn) =>
  Mutation.make(
    ~onCompleted,
    ~onError=e => Js.log2("Graphql Error:", e),
    innerComponentFn,
  );