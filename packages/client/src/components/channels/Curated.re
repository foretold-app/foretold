[@react.component]
let make = (~channel: Types.channel) =>
  channel.isCurated
    ? <div className="ant-tag ant-tag-blue"> {"Curated" |> Utils.ste} </div>
    : <Null />;