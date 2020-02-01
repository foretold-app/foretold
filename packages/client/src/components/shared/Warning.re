[@react.component]
let make = (~error) =>
  switch (error) {
  | None => <Null />
  | Some(error) => <Antd_Alert message={error |> Utils.ste} _type=`warning />
  };