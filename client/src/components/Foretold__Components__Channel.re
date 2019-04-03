open Utils;
open Style.Grid;

module SimpleHeader = {
  module Styles = {
    open Css;
    let header =
      style([
        borderBottom(`px(1), `solid, `hex("eee")),
        paddingLeft(`px(10)),
        paddingBottom(`em(0.8)),
        paddingRight(`em(0.4)),
        paddingTop(`px(10)),
        float(`left),
        width(`percent(100.)),
      ]);
  };

  let newMeasurable = channelId =>
    <Div float=`right>
      <Antd.Button
        onClick={_ => Context.Routing.Url.push(MeasurableNew(channelId))}>
        {"New Measurable" |> ste}
      </Antd.Button>
    </Div>;

  let editChannel = channelId =>
    <Div float=`right>
      <Antd.Button
        onClick={_ => Context.Routing.Url.push(ChannelEdit(channelId))}>
        {"Edit Channel" |> ste}
      </Antd.Button>
    </Div>;

  let leaveChannel = (channelId, agentId) =>
    Foretold__GraphQL.Mutations.ChannelLeave.Mutation.make((mutation, _) =>
      <Div float=`right>
        <Antd.Button
          onClick={
            _ =>
              Foretold__GraphQL.Mutations.ChannelLeave.mutate(
                mutation,
                agentId,
                channelId,
              )
          }>
          {"Leave Channel" |> ste}
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