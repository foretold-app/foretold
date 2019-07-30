let component = ReasonReact.statelessComponent("RedirectForAgentName");

let make = _children => {
  ...component,
  render: _ => {
    <Providers.AppContext.Consumer>
      ...{context => {
        Js.log2("Rounting Count:", context.routingCount);

        switch (context.loggedInUser, context.route, context.routingCount) {
        | (_, Profile, _) => ()
        | (Some(loggedInUser), _, 1) =>
          loggedInUser.agent
          |> E.O.fmap((agent: Types.agent) =>
               switch (agent.name) {
               | Some("") => Routing.Url.push(Profile)
               | _ => ()
               }
             )
          |> E.O.default()
        | _ => ()
        };

        ReasonReact.null;
      }}
    </Providers.AppContext.Consumer>;
  },
};