open Rationale;

/* Create an InMemoryCache */
let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

type headers = Js.t({. "authorization": string});

[@bs.deriving abstract]
type data = {name: string};

[@bs.scope "JSON"] [@bs.val]
external parseIntoMyData : string => data = "parse";

/* let result = parseIntoMyData("{\"authorization\": \"sdlkfjsdklfjskldfj\"}"); */

let headers =
  Json.Encode.(
    object_([
      (
        "authorization",
        Json.Encode.string(
          "Bearer " ++ (Auth0.authToken() |> Option.default("")),
        ),
      ),
    ])
  );

let httpLink =
  ApolloLinks.createHttpLink(
    ~uri="http://localhost:4000/graphql",
    ~headers,
    (),
  );

let errorLink =
  ApolloLinks.apolloLinkOnError(error => Js.log2("GraphQL Error!", error));
let link = ApolloLinks.from([|httpLink, errorLink|]);

let instance =
  ReasonApollo.createApolloClient(~link, ~cache=inMemoryCache, ());