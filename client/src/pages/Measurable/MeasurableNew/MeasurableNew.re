open Utils;
open Antd;
open MomentRe;

let ste = ReasonReact.string;

module CreateMeasurableMutation = {
  module GraphQL = [%graphql
    {|
             mutation createMeasurable($name: String!, $description: String!, $valueType:valueType!, $expectedResolutionDate:Date, $resolutionEndpoint: String!, $descriptionEntity: String!, $descriptionDate: Date, $descriptionProperty: String, $channel: String) {
                 createMeasurable(name: $name, description: $description, valueType: $valueType, expectedResolutionDate: $expectedResolutionDate, resolutionEndpoint: $resolutionEndpoint, descriptionEntity: $descriptionEntity, descriptionDate: $descriptionDate, descriptionProperty: $descriptionProperty, channel: $channel) {
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
      channel: string,
    ) => {
  let mutate =
    values.showDescriptionDate == "TRUE" ?
      CreateMeasurableMutation.GraphQL.make(
        ~name=values.name,
        ~description=values.description,
        ~descriptionProperty=values.descriptionProperty,
        ~expectedResolutionDate=
          values.expectedResolutionDate |> Js.Json.string,
        ~resolutionEndpoint=values.resolutionEndpoint,
        ~descriptionEntity=values.descriptionEntity,
        ~descriptionDate=values.descriptionDate |> Js.Json.string,
        ~channel,
        ~valueType=`FLOAT,
        (),
      ) :
      CreateMeasurableMutation.GraphQL.make(
        ~name=values.name,
        ~description=values.description,
        ~descriptionProperty=values.descriptionProperty,
        ~expectedResolutionDate=
          values.expectedResolutionDate |> Js.Json.string,
        ~resolutionEndpoint=values.resolutionEndpoint,
        ~descriptionEntity=values.descriptionEntity,
        ~channel,
        ~valueType=`FLOAT,
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

let make = (~channel, _children) => {
  ...component,
  render: _ => {
    let formCreator = mutation =>
      SignUpForm.make(
        ~onSubmit=({values}) => mutate(mutation, values, channel),
        ~initialState={
          name: "",
          description: "",
          descriptionProperty: "",
          descriptionEntity: "",
          expectedResolutionDate:
            MomentRe.momentNow() |> MeasurableForm.formatDate,
          descriptionDate: MomentRe.momentNow() |> MeasurableForm.formatDate,
          resolutionEndpoint: "",
          showDescriptionDate: "FALSE",
          showDescriptionProperty: "FALSE",
        },
        ~schema=[(`name, Custom(_ => None))],
      );

    <>
      <SLayout.Header>
        {SLayout.Header.textDiv("New Measurable")}
      </SLayout.Header>
      <SLayout.MainSection>
        {
          CreateMeasurableMutation.Mutation.make(
            ~onCompleted=e => Js.log("HI"),
            (mutation, data) =>
              formCreator(
                mutation,
                ({handleSubmit, handleChange, form, _}) => {
                  let showForm =
                    MeasurableForm.showForm(
                      ~form,
                      ~handleSubmit,
                      ~handleChange,
                    );
                  switch (data.result) {
                  | Loading => "Loading" |> ste
                  | Error(e) =>
                    <> {"Error: " ++ e##message |> ste} showForm </>
                  | Data(data) =>
                    data##createMeasurable
                    |> E.O.fmap(e => e##id)
                    |> doIfSome(_ => Urls.pushToLink(ChannelShow(channel)));
                    ReasonReact.null;
                  | NotCalled => showForm
                  };
                },
              )
              |> E.React.el,
          )
          |> E.React.el
        }
      </SLayout.MainSection>
    </>;
  },
};