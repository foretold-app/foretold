open Utils;
open Style.Grid;

module SimpleHeader = {
  module Styles = {
    let header =
      Css.style([
        Css.paddingLeft(`px(10)),
        Css.paddingBottom(`em(0.8)),
        Css.paddingRight(`em(0.4)),
        Css.paddingTop(`px(10)),
        Css.float(`left),
        Css.width(`percent(100.)),
      ]);
  };

  let buttonr = (str, action: Context.Routing.Url.t) =>
    <Div float=`right>
      <Antd.Button onClick={_ => Context.Routing.Url.push(action)}>
        {str |> ste}
      </Antd.Button>
    </Div>;

  let newMeasurable = channelId =>
    buttonr("New Question", MeasurableNew(channelId));

  let editChannel = channelId =>
    buttonr("Edit Channel", ChannelEdit(channelId));

  let newSeries = channelId => buttonr("New Series", SeriesNew(channelId));

  let inviteToChannel = channelId =>
    buttonr("Invite Members", ChannelInvite(channelId));

  let leaveChannel = (channelId: string) =>
    Foretold__GraphQL.Mutations.ChannelLeave.Mutation.make((mutation, _) =>
      <Div float=`right>
        <Antd.Button
          onClick={
            _ =>
              Foretold__GraphQL.Mutations.ChannelLeave.mutate(
                mutation,
                channelId,
              )
          }>
          {"Leave Channel" |> ste}
        </Antd.Button>
      </Div>
    )
    |> E.React.el;

  let joinChannel = channelId =>
    Foretold__GraphQL.Mutations.ChannelJoin.Mutation.make((mutation, _) =>
      <Div float=`right>
        <Antd.Button
          onClick={
            _ =>
              Foretold__GraphQL.Mutations.ChannelJoin.mutate(
                mutation,
                channelId,
              )
          }>
          {"Join Channel" |> ste}
        </Antd.Button>
      </Div>
    )
    |> E.React.el;

  let members = (channel: Context.Primary.Channel.t) =>
    channel.membershipCount
    |> E.O.React.fmapOrNull(c =>
         <Div float=`right>
           <Antd.Button
             onClick={
               _ => Context.Routing.Url.push(ChannelMembers(channel.id))
             }>
             <Icon.Icon icon="PEOPLE" />
             {c |> string_of_int |> ste}
           </Antd.Button>
         </Div>
       );
};