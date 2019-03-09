open Rationale;

/* Create an InMemoryCache */
let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

type headers = Js.t({. "authorization": string});

[@bs.deriving abstract]
type data = {name: string};

[@bs.scope "JSON"] [@bs.val]
external parseIntoMyData: string => data = "parse";

let headers = () =>
  Json.Encode.(
    object_([
      (
        "authorization",
        Json.Encode.string(
          "Bearer " ++ (Auth0.authToken() |> E.O.default("")),
        ),
      ),
    ])
  );

let httpLink = ApolloLinks.createHttpLink(~uri=Env.serverUrl, ());

let contextLink = ApolloLinks.createContextLink(() => {"headers": headers()});

let errorLink =
  ApolloLinks.apolloLinkOnError(error => Js.log2("GraphQL Error!", error));

let link = ApolloLinks.from([|contextLink, errorLink, httpLink|]);

let instance =
  ReasonApollo.createApolloClient(~link, ~cache=inMemoryCache, ());