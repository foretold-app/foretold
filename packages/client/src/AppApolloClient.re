let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

[@bs.deriving abstract]
type data = {name: string};

let storageToHeaders = (jwt: ServerJwt.t) =>
  Json.Encode.(
    object_([("authorization", Json.Encode.string("Bearer " ++ jwt))])
  );

let httpLink = ApolloLinks.createHttpLink(~uri=Env.serverUrl, ());

let contextLink = (tokens: ServerJwt.t) =>
  ApolloLinks.createContextLink(() => {"headers": storageToHeaders(tokens)});

let isUnauthenticated = error =>
  error##graphQLErrors
  |> Js.Nullable.toOption
  |> E.O.fmap(graphQLErrors =>
       graphQLErrors
       |> Js.Array.find(err =>
            err##extensions
            |> Js.Nullable.toOption
            |> E.O.fmap(extensions =>
                 extensions##code
                 |> Js.Nullable.toOption
                 |> E.O.fmap(code => code === "UNAUTHENTICATED")
                 |> E.O.default(false)
               )
            |> E.O.default(false)
          )
       |> E.O.fmap(_ => true)
       |> E.O.default(false)
     )
  |> E.O.default(false);

let isCode400 = error =>
  error##networkError
  |> Js.Nullable.toOption
  |> E.O.fmap(networkError => networkError##statusCode == 400)
  |> E.O.default(false);

let errorLink =
  ApolloLinks.apolloLinkOnError(error => {
    Js.log2("GraphQL Error!", Js.Json.stringifyAny(error));

    switch (Env.dev, error |> isUnauthenticated, error |> isCode400) {
    | (false, true, _)
    | (false, _, true) => Auth.Actions.logout()
    | _ => ()
    };
  });

let link = () =>
  switch (ServerJwt.make_from_storage()) {
  | Some(s) => ApolloLinks.from([|contextLink(s), errorLink, httpLink|])
  | None => ApolloLinks.from([|errorLink, httpLink|])
  };

/* TODO: Don't always load devtools. */
let instance = () =>
  ReasonApollo.createApolloClient(
    ~link=link(),
    ~cache=inMemoryCache,
    ~connectToDevTools=true,
    (),
  );
