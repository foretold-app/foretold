open Utils;

type t =
  | View
  | Invite;

let toS = (t: t) =>
  switch (t) {
  | View => "V"
  | Invite => "I"
  };

let button = (value, toUrl, str, id) =>
  <Antd.Button onClick={_ => Context.Routing.Url.push(toUrl(id))}>
    {str |> ste}
  </Antd.Button>;

let toUrl = (t: t, id: string): Context__Routing.Url.t =>
  switch (t) {
  | View => ChannelMembers(id)
  | Invite => ChannelInvite(id)
  };

let component = (agent, o: t, channel: Context.Primary.Channel.t) =>
  E.React.showIf(
    channel.myRole === Some(`ADMIN),
    button(Invite |> toS, Invite |> toUrl, "Invite", channel.id),
  );