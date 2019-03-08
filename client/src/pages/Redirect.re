open Utils;
open Rationale.Result.Infix;
open Rationale;
open E;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("Redirecting...");

let make = _children => {
  ...component,
  render: _ =>
    Queries.GetUser.component(
      Auth0.userId(),
      userQuery => {
        let agentId =
          userQuery
          |> O.bind(_, e => e##user)
          |> O.bind(_, e => e##agent)
          |> O.fmap(e => e##id);
        let name =
          userQuery |> O.bind(_, e => e##user) |> O.fmap(e => e##name);
        switch (name, agentId) {
        | (Some(""), _) => Urls.pushToLink(Profile)
        | (_, Some(id)) => Urls.pushToLink(AgentShow(id))
        | _ => ()
        };
        <div>
          <h1> {"Redirecting..." |> ste} </h1>
          <p>
            {
              "If you are not redirected shortly, try refreshing the page or contacting Ozzie."
              |> ste
            }
          </p>
        </div>;
      },
    ),
};