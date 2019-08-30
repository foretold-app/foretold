// This var need to prevent when "a user cannot go from profile page".
let redirectionCount = ref(0);

let defaultPage = (loggedInUser: Types.user) =>
  loggedInUser.agent
  |> E.O.bind(_, Primary.Agent.firstChannel)
  |> E.O.fmap((channel: Types.channel) => {
       Routing.Url.push(ChannelShow(channel.id));
       ReasonReact.null;
     })
  |> {
    Primary.Channel.globalLink()
    |> Routing.Url.toString
    |> ReasonReact.Router.push;
    E.O.default(<Home />);
  };

let component = ReasonReact.statelessComponent("Redirect");
let make = (~appContext: Providers.appContext, _children) => {
  ...component,
  didUpdate: _self => {
    redirectionCount := redirectionCount^ + 1;

    switch (appContext.loggedInUser, appContext.route, redirectionCount^) {
    | (_, Profile, _) => ()
    | (Some(loggedInUser), _, 2) =>
      loggedInUser.agent
      |> E.O.fmap((agent: Types.agent) =>
           switch (agent.name) {
           | Some("") =>
             Routing.Url.push(Profile);
             Antd.Message.info(
               ~content=Lang.nameIsEmpty |> Utils.ste,
               ~duration=30.,
               (),
             )
             |> ignore;
             ();
           | _ => ()
           }
         )
      |> E.O.default()
    | _ => ()
    };
  },

  render: _ => ReasonReact.null,
};