Application Layers:

- Node.js
- Express.js
- Appolo GraphQL
- GraphQL
- Middlewares
- Permissions
- Resolvers
- **DataSource**
- **AbstractModel**
- SequelizeModel
- PostgresQL

Caveats:
- Each layer knows only interface of nested layer, not deeper then only one.
  This means that **DataSource** know interface of **AbstractModel**, but
  does not know interface of SequelizeModel.
