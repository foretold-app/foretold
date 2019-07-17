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

module MeasurableReForm = ReForm.Create(MeasurableForm.Params);

let mutate =
    (
      mutation: CreateMeasurableMutation.Mutation.apolloMutation,
      values: MeasurableReForm.values,
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
        };

    CreateMeasurableMutation.GraphQL.make(~input, ());
  };

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
      MeasurableReForm.make(
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
          valueType: "FLOAT",
        },
        ~schema=[(`name, Custom(_ => None))],
      );

    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("New Question"),
      ~body=
        <FC.PageCard.BodyPadding>
          {CreateMeasurableMutation.Mutation.make((mutation, data) =>
             formCreator(mutation, ({handleSubmit, handleChange, form, _}) =>
               CMutationForm.showWithLoading2(
                 ~result=data.result,
                 ~form=
                   MeasurableForm.showForm(
                     ~form,
                     ~handleSubmit,
                     ~handleChange,
                   ),
                 ~onSuccess=
                   (response: CreateMeasurableMutation.GraphQL.t) => {
                     switch (response##measurableCreate) {
                     | Some(m) =>
                       Routing.Url.push(MeasurableShow(channelId, m##id))
                     };
                     ReasonReact.null;
                   },
                 (),
               )
             )
             |> E.React.el
           )
           |> E.React.el}
        </FC.PageCard.BodyPadding>,
    )
    |> layout;
  },
};