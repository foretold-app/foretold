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
     )
  |> E.O.toBool;

let errorLink =
  ApolloLinks.apolloLinkOnError(error => {
    Js.log2("GraphQL Error!", Js.Json.stringifyAny(error));

    switch (error |> isUnauthenticated) {
    | true => Auth.Actions.logout()
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