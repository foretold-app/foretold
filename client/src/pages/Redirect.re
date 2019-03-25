open Utils;
open E;
open Foretold__GraphQL;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("Redirecting...");

let make = _children => {
  ...component,
  render: _ =>
    Queries.User.withLoggedInUserQuery(user =>
      switch (user) {
      | Success(user) =>
        let agentId = user.agent |> O.fmap((e: DataModel.Agent.t) => e.id);
        let name = user.name;
        switch (name, agentId) {
        | ("", _) => DataModel.Url.push(Profile)
        | (_, Some(id)) => DataModel.Url.push(AgentShow(id))
        | _ => ()
        };
        <>
          {"Redirecting..." |> ste |> E.React.inH1}
          {
            "If you are not redirected shortly, try refreshing the page or contacting Ozzie."
            |> ste
            |> E.React.inP
          }
        </>;
      | _ => <div />
      }
    ),
};