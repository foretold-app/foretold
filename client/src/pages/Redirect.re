open Utils;
open E;
open Foretold__GraphQL;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("Redirecting...");

let make = _children => {
  ...component,
  render: _ =>
    Queries.User.component(
      Auth0.userId(),
      user => {
        let agentId =
          user
          |> O.bind(_, r => r.agent)
          |> O.fmap((e: Queries.User.agent) => e.id);
        let name = user |> O.fmap((e: Queries.User.user) => e.name);
        switch (name, agentId) {
        | (Some(""), _) => DataModel.Url.push(Profile)
        | (_, Some(id)) => DataModel.Url.push(AgentShow(id))
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