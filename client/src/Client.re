/* Create an InMemoryCache */
let inMemoryCache = ApolloInMemoryCache.createInMemoryCache(());

/* Create an HTTP Link */
let httpLink =
 ApolloLinks.createHttpLink(~uri="http://localhost:4000/graphql", ());

let instance = ReasonApollo.createApolloClient(
 ~link=httpLink, 
 ~cache=inMemoryCache, 
 ()
);