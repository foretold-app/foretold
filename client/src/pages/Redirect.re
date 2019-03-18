open Utils;
open E;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("Redirecting...");

let make = _children => {
  ...component,
  render: _ =>
    GetUser.component(
      Auth0.userId(),
      user => {
        let agentId =
          user
          |> O.bind(_, r => r.agent)
          |> O.fmap((e: GetUser.agent) => e.id);
        let name = user |> O.fmap((e: GetUser.user) => e.name);
        switch (name, agentId) {
        | (Some(""), _) => Urls.pushToLink(Profile)
        | (_, Some(id)) => Urls.pushToLink(AgentShow(id))
        | _ => ()
        };
        <>
          <h1> {"Redirecting..." |> ste} </h1>
          <p>
            {
              "If you are not redirected shortly, try refreshing the page or contacting Ozzie."
              |> ste
            }
          </p>
        </>;
      },
    ),
};