Application Layers:

- Node.js
- Express.js
- Appolo GraphQL
- GrpahQL
- Middlewares
- Permissions
- Resolvers
- **DataSource**
- **AbstractModel**
- SequelizeModel
- PostgresQL

Todos:
- [ ] Add interfaces for each bolded layers.
- [ ] Encapsulate SequelizeModels with AbstractModels.

Caveats:
- Each layer knows only interface of nested layer, not deeper then only one.
  This means that **DataSource** know interface of **AbstractModel**, but
  does not know interface of SequelizeModel.
- See interface to understand clearly.
