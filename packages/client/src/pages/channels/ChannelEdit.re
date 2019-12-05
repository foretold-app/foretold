[@bs.config {jsx: 3}];

open Rationale.Function.Infix;

module CMutationForm =
  MutationForm.Make({
    type queryType = ChannelUpdate.Query.t;
  });

[@react.component]
let make = (~channelId: string) => {
  let loadChannel = ChannelGet.getChannelByIdAsComponent(~id=channelId);

  let head =
    <>
      <FC.Base.Div float=`left>
        <FC.PageCard.HeaderRow.Title>
          {"Edit Community" |> Utils.ste}
        </FC.PageCard.HeaderRow.Title>
      </FC.Base.Div>
      <Providers.AppContext.Consumer>
        ...{({loggedUser}) =>
          switch (loggedUser) {
          | Some(loggedUser) =>
            <FC.Base.Div
              float=`right
              className={Css.style([
                FC.PageCard.HeaderRow.Styles.itemTopPadding,
              ])}>
              {Primary.User.show(
                 loggedUser,
                 <FC.Base.Button
                   variant=FC.Base.Button.Primary
                   size=FC.Base.Button.Small
                   onClick={e =>
                     LinkType.onClick(Internal(SeriesNew(channelId)), e)
                   }>
                   {"New Series" |> Utils.ste}
                 </FC.Base.Button>,
               )}
            </FC.Base.Div>
          | _ => <Null />
          }
        }
      </Providers.AppContext.Consumer>
    </>;

  let form = (mutation, channel: Types.channel, clb) =>
    ChannelForm.Form.use(
      ~onSubmit=
        values => {
          ChannelUpdate.mutate(
            mutation,
            channelId,
            values.state.values.name,
            values.state.values.description,
            values.state.values.isPublic |> E.Bool.fromString,
            values.state.values.isArchived |> E.Bool.fromString,
          );
          None;
        },
      ~initialState={
        name: channel.name,
        description: channel.description |> E.O.default(""),
        isPublic: channel.isPublic |> E.Bool.toString,
        isArchived: channel.isArchived |> E.Bool.toString,
      },
      ~schema=ChannelForm.Form.Validation.Schema([||]),
      (),
    )
    |> clb;

  <SLayout head>
    <FC.PageCard.BodyPadding>
      {loadChannel(
         HttpResponse.fmap(channel =>
           <ChannelUpdate.Mutation>
             ...{(mutation, data) =>
               form(
                 mutation,
                 channel,
                 ({submit, handleChange, state}: ChannelForm.Form.api) =>
                 CMutationForm.showWithLoading(
                   ~result=data.result,
                   ~form=
                     ChannelForm.showForm(
                       ~state,
                       ~handleChange,
                       ~creating=false,
                       ~onSubmit=() => submit(),
                       (),
                     ),
                   ~successMessage="Community edited successfully.",
                   (),
                 )
               )
             }
           </ChannelUpdate.Mutation>
         )
         ||> HttpResponse.withReactDefaults,
       )}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};