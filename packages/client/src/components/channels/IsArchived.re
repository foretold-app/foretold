[@react.component]
let make = (~channel: Types.channel) =>
  channel.isArchived
    ? <div className="ant-tag ant-tag-gold"> {"Archived" |> Utils.ste} </div>
    : <Null />;