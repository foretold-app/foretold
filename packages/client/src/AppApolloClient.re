[@bs.config {jsx: 3}];

let inMemoryCache = () => ApolloInMemoryCache.createInMemoryCache();

let storageToHeaders = (jwt: ServerJwt.t) =>
  Json.Encode.(
    object_([("authorization", Json.Encode.string("Bearer " ++ jwt))])
  );

let httpLink = _ => ApolloLinks.createHttpLink(~uri=Env.serverUrl, ());

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

let errorLink = _ =>
  ApolloLinks.apolloLinkOnError(error => {
    Js.log2("GraphQL Error!", Js.Json.stringifyAny(error));

    switch (error |> isUnauthenticated, error |> isCode400, Env.clientEnv) {
    | (true, _, _)
    | (_, true, Production) => Auth.Actions.logout()
    | _ => ()
    };
  });

let link = serverJwt =>
  switch (serverJwt) {
  | Some(s) => ApolloLinks.from([|contextLink(s), errorLink(), httpLink()|])
  | None => ApolloLinks.from([|errorLink(), httpLink()|])
  };

let connectToDevTools = _ => {
  switch (Env.clientEnv) {
  | Development => true
  | _ => false
  };
};

let instance = serverJwt =>
  ReasonApollo.createApolloClient(
    ~link=link(serverJwt),
    ~cache=inMemoryCache(),
    ~connectToDevTools=connectToDevTools(),
    (),
  );
