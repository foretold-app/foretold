open Rationale.Function.Infix;

let ste = ReasonReact.string;

module ChannelFormShower = ReForm.Create(ChannelForm.NewChannelParams);

module Mutation = Foretold__GraphQL.Mutations.ChannelUpdate;

module CMutationForm =
  MutationForm.Make({
    type queryType = Mutation.Query.t;
  });

let component = ReasonReact.statelessComponent("ChannelNewPage");

let make = (~channelId: string, ~layout, _children) => {
  ...component,
  render: _ => {
    let loadChannel =
      Foretold__GraphQL.Queries.Channel.component2(~id=channelId);

    let mutationMake =
      Mutation.Mutation.make(~onCompleted=_ => ()) ||> E.React.el;

    let header = SLayout.Header.textDiv("Edit Channel");

    let form = (mutation, channel: Context.Primary.Channel.t) =>
      ChannelFormShower.make(
        ~onSubmit=
          ({values}) =>
            Mutation.mutate(
              mutation,
              channelId,
              values.name,
              Some(values.description),
              values.isPublic |> E.Bool.fromString,
            ),
        ~initialState={
          name: channel.name,
          description: channel.description |> E.O.default(""),
          isPublic: channel.isPublic |> E.Bool.toString,
        },
        ~schema=[(`name, Custom(_ => None))],
      )
      ||> E.React.el;

    loadChannel(
      E.HttpResponse.fmap(result =>
        mutationMake((mutation, data) =>
          form(mutation, result, ({handleSubmit, handleChange, form, _}) =>
            CMutationForm.showWithLoading(
              ~result=data.result,
              ~form=ChannelForm.showForm(~form, ~handleSubmit, ~handleChange),
              ~successMessage="Channel edited successfully.",
              (),
            )
          )
        )
      )
      ||> E.HttpResponse.withReactDefaults,
    )
    |> SLayout.LayoutConfig.make(~head=header, ~body=_)
    |> layout;
  },
};