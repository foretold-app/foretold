[@react.component]
let make = (~error) =>
  switch (error) {
  | None => <Null />
  | Some(error) => <AntdAlert message=error type_="warning" />
  };