[@react.component]
let make = (~children=<Null />) => {
  let context = React.useContext(Providers.app);
  switch (context.loggedUser) {
  | Some(loggedUser) => Primary.User.show(loggedUser, children)
  | _ => <Null />
  };
};