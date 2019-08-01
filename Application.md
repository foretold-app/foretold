### Application
- Server Side Part
- Client Part (UI)
- Application State Part (DBS)

### Terms
- **Application** - is the Foretold application;
- **System** (Application System) - is a group of Objects, Subjects, Process 
and Flows which works together and allows the application to work as expected;
- **Late User Binding** - it is a process with binding application "User" with 
a "Auth0 User", first of all "User" is added in the application and only 
then attached with "Auth0 User";
- **Authorization**
- **Authentication**
- **Identification**
- Event
- Job - see async subsystem;
- Object Definition
- Association Description
- Brier Score
- CDF
- JWT
- Migration
- Seed

### Graphql Terms
- Connection - edges, nodes;
- Type
- Middleware
- Resolver

### Objects
- Agent
- User
- Bot
- Channel
- Measurable
- Measurement
- Series
- Preference
- Token
- Template
- Notification
- Invitation
- Agent Notification
- Channel Membership

### Server Side Layers
- **Data Layer**
- **Models Layer** - object definitions, async hooks definitions, association 
descriptions;
- **Models Abstractions Layer**
- **Graphql Layer** - includes Resolvers, Middlewares, Authorizers.

### Server Side Subsystems
- **Async Subsystems** - contains events, jobs, listeners, emitters;
- **Notification Subsystem** - contains producers, consumers;
- **Graphql Subsystem** - contains types, resolvers, middlewares, authorizers;
- **Data Subsystem**

### Server Side Requirements
- Heroku.com
- Backup / Restore
- **Platforms** - Node.js 12.6.0;
- **DBS** - PostresQL 11

### External System Requirements
- Auth0.com
- Amazon AWS SES
