let component = ReasonReact.statelessComponent("Redirect");

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
               | Some("") =>
                 Routing.Url.push(Profile);
                 Antd.Message.info(~content=Lang.nameIsEmpty |> Utils.ste, ())
                 |> ignore;
                 ();
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