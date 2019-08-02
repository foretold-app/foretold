### Application
- Server Side Part
- Client Part (UI)
- Application State Part (DBS)

### Terms
- **Application** - is the Foretold application;
- **System** (Application System) - is a group of Objects, Subjects, Process 
and Flows which works together and allows the application to work as expected;
- **Authorization**
- **Authentication**
- **Identification**
- Event
- Job - see async subsystem;
- Object Definition
- Association Description
- Brier Score
- CDF
- **Server Side JWT** - is used in API calls;
- **One-time Token** - has an limit of life time;
- **Long-term Token** - has no life time limits;
- Migration
- Seed
- **Auth0 JWT** - is used ...;
- **Auth0 Access Token** - is used ...;
- **Auth0.com** - is an external system to identify users; 

### Graphql Terms
- Connection - edges, nodes;
- Type
- Middleware
- Resolver

### Objects
- **Agent** - is an abstraction of User, Bot objects;
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

### Authentication Flow
1. Get "Server Side JWT" using (one of):
- "Auth0 JWT", "Auth0 Access Token"
- "One-time Token"
- "Long-term Token"
2. Get API responses with "Server Side JWT".