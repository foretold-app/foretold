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
  <Antd.Button onClick={_ => Context.Routing.Url.push(toUrl(id))}>
    {str |> ste}
  </Antd.Button>;

let component = (o: t, channel: Context.Primary.Channel.t) =>
  <div>
    {button(Edit |> toS, Edit |> toUrl, "Edit", channel.id)}
    {button(NewSeries |> toS, NewSeries |> toUrl, "New Series", channel.id)}
  </div>;