[@react.component]
let make = (~channel: Types.channel) =>
  channel.description
  |> E.O.React.fmapOrNull(description => <ReactMarkdown source=description />);