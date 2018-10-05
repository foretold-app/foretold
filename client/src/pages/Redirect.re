open Utils;
open Rationale.Result.Infix;
open Rationale;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("Redirecting...");

let withUserQuery =
    (
      auth0Id: option(string),
      innerComponentFn: 'a => ReasonReact.reactElement,
    ) =>
  switch (auth0Id) {
  | Some(auth) =>
    let query = Queries.GetUser.make(~auth0Id=auth, ());
    Queries.GetUserQuery.make(~variables=query##variables, ({result}) =>
      result
      |> apolloResponseToResult
      <$> (e => innerComponentFn(Some(e)))
      |> Rationale.Result.result(idd, idd)
    )
    |> ReasonReact.element;
  | None => innerComponentFn(None)
  };

let omap = Rationale.Option.fmap;
let make = _children => {
  ...component,
  render: _ =>
    withUserQuery(
      Auth0.userId(),
      userQuery => {
        let agentId =
          userQuery
          |> Rationale.Option.bind(_, e => e##user)
          |> Rationale.Option.bind(_, e => e##agent)
          |> omap(e => e##id);
        let name =
          userQuery
          |> Rationale.Option.bind(_, e => e##user)
          |> omap(e => e##name);
        if (name == Some("")) {
          ReasonReact.Router.push("/profile");
        } else {
          switch (agentId) {
          | Some(i) => ReasonReact.Router.push("/agents/" ++ i)
          | _ => ()
          };
        };
        <div>
          <h1> ("Redirecting..." |> ste) </h1>
          <p>
            (
              "If you are not redirected shortly, try refreshing the page or contacting Ozzie."
              |> ste
            )
          </p>
        </div>;
      },
    ),
};