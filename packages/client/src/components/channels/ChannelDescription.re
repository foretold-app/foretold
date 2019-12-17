// To truncate the MarkDown it is not a simple issue.
// So just let us truncate text by words.

[@react.component]
let make = (~channel: Types.channel) =>
  channel.description
  |> E.O.React.fmapOrNull(description =>
       <Markdown source={Utils.truncateByWords(description)} />
     );