[@react.component]
let make = (~children=<Null />) => {
  let context = React.useContext(Providers.app);
  switch (context.loggedUser) {
  | Some(_) => <Null />
  | None => children
  };
};