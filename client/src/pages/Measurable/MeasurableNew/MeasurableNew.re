open Utils;
open Antd;
open MomentRe;

let ste = ReasonReact.string;

module CreateMeasurableMutation = {
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
};

module SignUpForm = ReForm.Create(MeasurableForm.SignUpParams);

let mutate =
    (
      mutation: CreateMeasurableMutation.Mutation.apolloMutation,
      values: SignUpForm.values,
      channelId: string,
    ) => {
  let mutate =
    values.showDescriptionDate == "TRUE" ?
      CreateMeasurableMutation.GraphQL.make(
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
      ) :
      CreateMeasurableMutation.GraphQL.make(
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
    ~refetchQueries=[|"getMeasurables"|],
    (),
  )
  |> ignore;
};

let component = ReasonReact.statelessComponent("Measurables");

module CMutationForm =
  MutationForm.Make({
    type queryType = CreateMeasurableMutation.GraphQL.t;
  });

let make = (~channelId, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ => {
    let formCreator = mutation =>
      SignUpForm.make(
        ~onSubmit=({values}) => mutate(mutation, values, channelId),
        ~initialState={
          name: "",
          labelCustom: "",
          labelProperty: "",
          labelSubject: "",
          expectedResolutionDate:
            MomentRe.momentNow() |> MeasurableForm.formatDate,
          labelOnDate: MomentRe.momentNow() |> MeasurableForm.formatDate,
          resolutionEndpoint: "",
          showDescriptionDate: "FALSE",
          showDescriptionProperty: "FALSE",
        },
        ~schema=[(`name, Custom(_ => None))],
      );

    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("New Question"),
      ~body=
        CreateMeasurableMutation.Mutation.make(
          ~onCompleted=
            () => Context.Routing.Url.push(ChannelShow(channelId)),
          (mutation, data) =>
            formCreator(mutation, ({handleSubmit, handleChange, form, _}) =>
              CMutationForm.showWithLoading(
                ~result=data.result,
                ~form=
                  MeasurableForm.showForm(
                    ~form,
                    ~handleSubmit,
                    ~handleChange,
                  ),
                (),
              )
            )
            |> E.React.el,
        )
        |> E.React.el,
    )
    |> layout;
  },
};