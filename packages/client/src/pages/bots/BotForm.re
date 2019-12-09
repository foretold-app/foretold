[@bs.config {jsx: 3}];
open BsReform;

module FormConfig = [%lenses
  type state = {
    name: string,
    description: string,
    competitorType: Types.competitorType,
    picture: string,
  }
];

module Form = ReForm.Make(FormConfig);

let schema =
  Form.Validation.Schema([|
    Custom(
      Name,
      values =>
        Js.String.length(values.name) > 64
          ? ReSchema.Error("Keep it short!") : Valid,
    ),
    Custom(
      Name,
      values =>
        Js.String.length(values.name) < 3
          ? Error("The name too short.") : Valid,
    ),
  |]);

module FieldString = {
  [@react.component]
  let make = (~field, ~label) => {
    <Form.Field
      field
      render={({handleChange, error, value, validate}) =>
        <Antd.Form.Item label={label |> Utils.ste}>
          <Antd.Input
            value
            onChange={BsReform.Helpers.handleChange(handleChange)}
            onBlur={_ => validate()}
          />
          <p> {error->Belt.Option.getWithDefault("") |> Utils.ste} </p>
        </Antd.Form.Item>
      }
    />;
  };
};

let onSuccess = (loggedUser: Types.user) => {
  Primary.User.getAgent(loggedUser, agent =>
    Routing.Url.push(Agent({agentId: agent.id, subPage: AgentBots}))
  );

  <Null />;
};

module FormComponent = {
  [@react.component]
  let make =
      (
        ~reform: Form.api,
        ~result: ReasonApolloHooks.Mutation.controledVariantResult('a),
      ) => {
    let onSubmit = event => {
      ReactEvent.Synthetic.preventDefault(event);
      reform.submit();
    };

    <Form.Provider value=reform>
      {switch (result) {
       | Error(_error) => <p> {"Something went wrong..." |> Utils.ste} </p>
       | Data(_) =>
         <Providers.AppContext.Consumer>
           ...{({loggedUser}) =>
             switch (loggedUser) {
             | Some(loggedUser) => onSuccess(loggedUser)
             | _ => <Null />
             }
           }
         </Providers.AppContext.Consumer>
       | _ =>
         <FC__PageCard.BodyPadding>
           <Antd.Form onSubmit>
             <FieldString field=FormConfig.Name label="Name" />
             <FieldString field=FormConfig.Description label="Description" />
             <FieldString field=FormConfig.Picture label="Picture" />
             <Antd.Form.Item>
               {reform.state.formState == Submitting
                  ? <Spin />
                  : <Antd.Button _type=`primary onClick=onSubmit>
                      {"Submit" |> Utils.ste}
                    </Antd.Button>}
             </Antd.Form.Item>
           </Antd.Form>
         </FC__PageCard.BodyPadding>
       }}
    </Form.Provider>;
  };
};

module Create = {
  [@react.component]
  let make = () => {
    let (mutate, result, _) = BotCreateMutation.Mutation.use();

    let reform =
      Form.use(
        ~validationStrategy=OnDemand,
        ~schema,
        ~onSubmit=
          ({state}) => {
            mutate(
              ~variables=
                BotCreateMutation.Query.make(
                  ~input={
                    "name": state.values.name,
                    "description": Some(state.values.description),
                    "competitorType": state.values.competitorType,
                    "picture": Some(state.values.picture),
                  },
                  (),
                )##variables,
              (),
            )
            |> ignore;

            None;
          },
        ~initialState={
          name: "",
          description: "",
          competitorType: `COMPETITIVE,
          picture: "",
        },
        (),
      );

    <FormComponent reform result />;
  };
};

module Edit = {
  [@react.component]
  let make = (~bot: Types.bot) => {
    let (mutate, result, _) = BotUpdate.Mutation.use();

    let reform =
      Form.use(
        ~validationStrategy=OnDemand,
        ~schema,
        ~onSubmit=
          ({state}) => {
            mutate(
              ~variables=
                BotUpdate.Query.make(
                  ~id=bot.id,
                  ~input={
                    "name": state.values.name,
                    "description": Some(state.values.description),
                    "competitorType": state.values.competitorType,
                    "picture": Some(state.values.picture),
                  },
                  (),
                )##variables,
              (),
            )
            |> ignore;

            None;
          },
        ~initialState={
          name: bot.name |> Rationale.Option.default(""),
          description: bot.description |> Rationale.Option.default(""),
          competitorType: `COMPETITIVE,
          picture: bot.picture |> Rationale.Option.default(""),
        },
        (),
      );

    <FormComponent reform result />;
  };
};