# Concepts
- We widely use "Types" for both sides. As for the server as well as for the client
sides. See "Cursor" types for instance. Do not use ordinary "string" types
it does not tell us something, you wanted to say something certain. And all
strings have length. Do not forget about it.
- We do not use the cache. The time has not come.
- We like encapsulating flows.
- We do not deliver business logic to the DB side. 
- We like using abilities of full technology stack (Node.js, Postgres, React 
and so on).
- We do not like comments in the code. Use code as comments.
- The better code is the code does not exist.
- We always remember about timezones. We store all time formats as UTC.
- We hate code duplication.

# Introduction
To understand better how the application works, which data flows are existed in
it, pay attention on this text. Please, start from **Terms**, 
and **Objects**.

Some solutions are made to give ability in the future to grow application and
prepare it to work in multi-instance mode (DB - PostgreSQL, Data/Models Layers,
Async/Sync parts, Notifications Subsystem and so on), to be ready to scale application
**by X, Y, Z** axes of scaling model. Where **X axis** scaling is horizontal duplication 
(scale by cloning), **Y axis** - functional decomposition (splitting different things),
**Z axis** - data partitioning (scale by splitting similar things).

Talking about Z axis. In DB application uses "UUIDV4" as ID of Objects. It is not
sortable since it is not sequence, but it is useful in splitting data by 
shards. Maybe in the future we are going to use "MongoDB Object ID" as ID of objects
right on PostreSQL side. But do not forget that to migrate old data we should
order it with "createdAt" field and only then give each object new ID (if it will
need).

Using UUID as ID of DB tables rows is a brilliant idea. Because each row
represents Objects. Hence each Object has its own an identificator (ID). Do really
like this aspect.

### Application Flows
1. Measurables States Flow. 
"Measurables" have "states". Some part of the app moves "Open" measurables
to "Judgement Pending" measurables. Another part moves from "Judgement Pending"
to "Judged" or "Agent" does it. 

2. Aggregation Measurements Flow. Bots Flow.
"Measurements" are being made by "Agents" though the client. They are called
"Competitive". If them make "Bots" these are "Aggregation" measurements. So each
time period some "Bots" make "Aggregations" of previous "Measurements".

3. Authentication Flow (to describe how application identifies users).
What does "Authentication Primary" mean?
What does "Authentication Secondary" mean? And what is the difference 
between these two entities?

4. Authorization Flow (to describe how queries go through access control).

5. Logs Flow. This flow works with Google Stack Drive. Only used for
maintenance purpose.

6. User Info Updating Flow. This flow uses "access token" of "Auth0.com" and 
updated users info.

7. Notification Producing Flow.

8. Notification Consuming Flow.

9. Objects Co-creation Flow. 

10. Invitations Flow.

11. Mails Sender Flow.
How this works?

12. Feed Items Flow. 
How are they created?

13. Ken Library Flow. GitHub Integration Description.

14. GraphQL Errors Masking Caveats.
In production mode "Graphql Apollo Server" always masks all errors. This does
not correspond to "Graphql Apollo Server" this tied with "Graphql Protocol".


### Authentication Flow
1. Get "Server Side JWT" using (one of):
- "Auth0 JWT", "Auth0 Access Token".
- "One-time Token".
- "Long-term Token".
2. Get API responses with "Server Side JWT".

### Application
- Server Side Part.
- Client Part (UI).
- Application State Part (DBS).

### Common System Terms
- **Application** - is the Foretold application.
- **System** (Application System) - is a group of Objects, Subjects, Process 
and Flows which works together and allows the application to work as expected.
- **Authorization**.
- **Authentication**.
- **Identification**.
- **Event** - part of async application life, encapsulated system event, like 
"new member in some channel".
- **Job** - see async subsystem, encapsulated part of application which uses
some resources of application.
- **Object Definition** - any Object descriptions (JSON or similar).
- **Association Description** - any definition of "Has Many", "Has One", "Has and
belongs to many" (like JSON, or SQL representation).
- **Brier Score** - score of predictions.
- **CDF** - Cumulative distribution function.
- **PDF**.
- **Server Side JWT** - is used in API calls to **Identify** application objects.
- **One-time Token** - has an limit of life time.
- **Long-term Token** - has no life time limits.
- **Migration** - used to set DataBase according to application stage.
- **Seed** - part of application to prepare some set of data to show how
the application works.
- **Auth0 JWT**.
- **Auth0 Access Token**.
- **Auth0.com** - is an external system to identify users. 
- **Association** - it is a link between objects represents real world objects
associations, for instance Each School has its own Students ("has many" in 
this case).
- **Competitive Measurement** - "Agents" do it thought the client.
- **Aggregation Measurement** - result of "Bots" work.
- **Unresolved Measurement**.

### Graphql Terms
- Connection - edges, nodes.
- Type.
- Middleware.
- Resolver.
- Authorizer.

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

- **Agent** (clear*) - is an abstraction of User, Bot objects.
- **User** (combined*) - represents a User of Application.
- **Bot** (combined*) - each User can create a Bot.
- **Channel** (combined*) - all data are separated by channels.
- **Measurable** (combined*) - **questions** about probability.
- **Measurement** (combined*) - **predictions**, results of measurements 
for questions.
- **Series** (combined*) - series of measurements.
- **Preference** (combined*) - each Agent has it own preferences 
or settings for a client.
- **Token** (combined*) - abstraction for tokens.
- **Template** (clear*) - abstraction for templates for any use-case.
- **Notification** (clear*) - part of Notification Subsystem.
- **Invitation** (combined*) - abstraction of Invitation Flow.
- **Notification Status** (link*) - links between Agents and Notifications.
- **Channel Membership** (link*) - links between Channels
and theirs Members (Agents).
- **Global Setting**.
- **Feed Item**.

Marks: 
_clear*_ - it is an object without any external links (like  "agent_id" or 
similar),
_link*_ - an object to link another objects, 
_combined*_ - contains Link and Clear aspects of Objects.

### Server Side Layers
- **Data Layer**.
- **Models Layer** - object definitions, async hooks definitions, association 
descriptions.
- **Models Abstractions Layer**.
- **Graphql Layer** - includes Resolvers, Middlewares, Authorizers.

### Server Side Subsystems
- **Sync Subsystems** - conglomeration of system reactions.
- **Notification Subsystem** - contains producers, consumers.
- **Graphql Subsystem** - external interface (API).
- **Data Subsystem** - data manipulations (business logic).

### Server Side Requirements
- Heroku.com (backend CI).
- Travis for CD/CI tests (https://travis-ci.org).
- Backup / Restore (now done via Heroku).
- **Platforms** - Node.js 12.6.0.
- **DBS** - PostresQL 11.

### External System Requirements
- Auth0.com - clients external authentication.
- Amazon AWS SES - email provider.
- Google Stack Drive - log management.

### Some Words About Hooks

See: "packages/server/src/models/hooks.js" file. Try to keep all "sync" hooks 
in one place. For what do you want to ask?

The server side application architecture is being building with separated,
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

### Why do we use plain classes like "Data", "Params" and so on?

We use "Data", "Params", "Filter", "Options", "Pagination", "Query",
"Response All", "Restrictions" classes. 

- The main purpose is to protect any developer from widely counted interface errors.
- The second reason is that we want to increase readable qualities of the code.
