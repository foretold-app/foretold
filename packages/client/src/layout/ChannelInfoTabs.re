open Utils;
type t =
  | NewSeries
  | Edit;

let toS = (t: t) =>
  switch (t) {
  | NewSeries => "N"
  | Edit => "E"
  };

let toUrl = (t: t, id: string): Context__Routing.Url.t =>
  switch (t) {
  | NewSeries => SeriesNew(id)
  | Edit => ChannelEdit(id)
  };

let button = (value, toUrl, str, id) =>
  <Antd_Radio_Button
    value onClick={_ => Context.Routing.Url.push(toUrl(id))}>
    {str |> ste}
  </Antd_Radio_Button>;

let component = (o: t, channel: Context.Primary.Channel.t) =>
  <Antd.Radio.Group defaultValue="foo" value={o |> toS} onChange={e => ()}>
    {button(Edit |> toS, Edit |> toUrl, "Edit", channel.id)}
    {button(NewSeries |> toS, NewSeries |> toUrl, "New Series", channel.id)}
  </Antd.Radio.Group>;