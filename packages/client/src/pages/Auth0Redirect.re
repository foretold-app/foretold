open Utils;
open E;
open Foretold__GraphQL;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("Redirecting...");

let make = (~loggedInUser: option(Context__Primary.User.t), _children) => {
  ...component,
  render: _ =>
    switch (loggedInUser) {
    | Some(userData) =>
      let user = userData;
      let agentId =
        user.agent |> O.fmap((e: Context.Primary.Agent.t) => e.id);
      let name = user.name;
      switch (name, agentId) {
      | ("", _) => Context.Routing.Url.push(Profile)
      | (_, Some(id)) =>
        Context.Routing.Url.push(Agent({agentId: id, subPage: AgentShow}))
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
    | _ =>
      Context.Routing.Url.push(Home);
      <div />;
    },
};