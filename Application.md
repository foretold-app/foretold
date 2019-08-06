To understand better how the application works, which data flows are existed in
it, pay attention on this text. Please, start from **Terms**, 
and **Objects**.

Some solutions are made to give ability in the future to grow application and
prepare it to work in multi-instance mode (DB - PostgreSQL, Data/Models Layers,
Async/Sync parts, Notifications Subsystem and so on), to be ready to scale application
by X, Y, Z axes of scaling model. Where X axis scaling is horizontal duplication 
(scale by cloning), Y axis - functional decomposition (splitting different things),
Z axis - data partitioning (scale by splitting similar things).

Talking about Z axis. In DB application uses "UUIDV4" as ID of Objects. It is not
sortable since it is not sequence, but it is useful in splitting data by 
shards. Maybe in the future we are going to use "MongoDB Object ID" as ID of objects
right on PostreSQL side. But do not forget that to migrate an old data we should
order it with "createdAt" field and only then give each object new ID (if it will
need).

### Application
- Server Side Part
- Client Part (UI)
- Application State Part (DBS)

### Common System Terms
- **Application** - is the Foretold application;
- **System** (Application System) - is a group of Objects, Subjects, Process 
and Flows which works together and allows the application to work as expected;
- **Authorization**
- **Authentication**
- **Identification**
- **Event** - part of async application life, encapsulated system event, like 
"new member in some channel";
- **Job** - see async subsystem, encapsulated part of application which uses
some resources of application;
- **Object Definition** - any Object descriptions (JSON or similar);
- **Association Description** - any definition of "Has Many", "Has One", "Has and
belongs to many" (like JSON, or SQL representation);
- **Brier Score** - score of predictions;
- **CDF** - Cumulative distribution function;
- **Server Side JWT** - is used in API calls to **Identify** application objects;
- **One-time Token** - has an limit of life time;
- **Long-term Token** - has no life time limits;
- **Migration** - used to set DataBase according to application stage;
- **Seed** - part of application to prepare some set of data to show how
the application works;
- **Auth0 JWT** - is used ...;
- **Auth0 Access Token** - is used ...;
- **Auth0.com** - is an external system to identify users; 
- **Association** - it is a link between objects represents real world objects
associations, for instance Each School has its own Students ("has many" in 
this case).

### Graphql Terms
- Connection - edges, nodes;
- Type
- Middleware
- Resolver

### Objects

Rules:

1. Always try to keep objects as clear as you can, try to not use Links
like "agent_id", "channel_id" or so on. It will prevent you from many
future issues when you are going to refactor the system more.

2. To prevent hard link between "logic flows" you can add something
like "methodCreatedBy" of "ChannelMembership". So, for what do
you think I am doing this? The main reason is to separate Sync/Async flows
of application. At the right hand you just want to store new membership
in "ChannelMembership" table, on another hand and you do not want to hard link this 
part of application with async flow "when somebody is being added into channel".
In this case just add special aspect of an object and then (later) use this
aspect in async flow, just make another system event like "New Channel 
Membership" is added. 

3. Each System Object must have "createAt", "updateAt" timestamp. And if there is 
 an ability to add this "updatedAt" too (on the next stage).
 
4. Objects have "associations". In this applications they are stored in 
"Models Definition" layer and in the DB side in "constraints". 

- **Agent** (clear*) - is an abstraction of User, Bot objects;
- **User** (combined*) - represents a User of Application;
- **Bot** (combined*) - each User can create a Bot;
- **Channel** (combined*) - all data are separated by channels;
- **Measurable** (combined*) - questions about probability;
- **Measurement** (combined*) - results of measurements for questions;
- **Series** (combined*) - series of measurements;
- **Preference** (combined*) - each Agent has it own preferences 
or settings for a client;
- **Token** (combined*) - abstraction for tokens;
- **Template** (clear*) - abstraction for templates for any use-case;
- **Notification** (clear*) - part of Notification Subsystem;
- **Invitation** (combined*) - abstraction of Invitation Flow;
- **Agent Notification** (link*) - links between Agents and Notifications;
- **Channel Membership** (link*) - links between Channels 
and theirs Members (Agents).

Marks: 
_clear*_ - it is an object without any external links (like  "agent_id" or 
similar),
_link*_ - an object to link another objects, 
_combined*_ - contains Link and Clear aspects of Objects.

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

### Some Words About Hooks

See: "packages/server/src/models/hooks.js" file.

Try to keep all "async" hooks in one place.
For what do you want to ask?

Server side application architecture is being building with separated,
decomposed layers (Model Definitions, Models Abstractions, Data Layer.
Middleware Layer, Authorizers Layer, Async Flows Layer, GrqphQL
Layer ans its resolvers).

Models are just "data type definitions" which are similar with "grapqhl
types". "Sequelize" is a just lib which plays role of "DB Driver".

So this is why we keep "async" part of the application here, and
"associations" definitions too. It does not matter where the "associations"
are defined and placed (either DB or JS files). The main purpose
is to give to developers to read this base definitions without
duplications and keep in a mind only a small part of this.

