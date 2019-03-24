open Belt;
open Utils;

let logout = () => {
  Me.AuthTokens.destroy();
  ReasonReact.Router.push("/");
  ();
};

let logoutIfTokenIsObsolete = (tokens: Me.AuthTokens.t) =>
  if (tokens |> Me.AuthTokens.isObsolete) {
    logout();
  };

let userId = () =>
  Me.AuthTokens.make_from_storage() |> E.O.bind(_, Me.AuthTokens.auth0Id);