[@bs.config {jsx: 3}];

module Query = [%graphql
  {|
            mutation botCreate($input: BotInput!) {
                botCreate(input: $input) {
                 id
                }
            }
    |}
];

module Mutation = ReasonApolloHooks.Mutation.Make(Query);

module FieldString = {
  [@react.component]
  let make = (~field, ~label) => {
    <BotForm.Form.Field
      field
      render={({handleChange, error, value, validate}) =>
        <label>
          <span> {React.string(label)} </span>
          <input
            value
            onChange={BsReform.Helpers.handleChange(handleChange)}
            onBlur={_ => validate()}
          />
          <p> {error->Belt.Option.getWithDefault("")->React.string} </p>
        </label>
      }
    />;
  };
};

[@react.component]
let make = () => {
  let (mutate, result, _) = Mutation.use(); // ~client

  let reform =
    BotForm.Form.use(
      ~validationStrategy=OnDemand,
      ~schema={
        BotForm.Form.Validation.Schema([|
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
      },
      ~onSubmit=
        ({state}) => {
          mutate(
            ~variables=
              Query.make(
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

  <BotForm.Form.Provider value=reform>
    {switch (result) {
     | Error(_error) => <p> {React.string("Something went wrong...")} </p>
     | Data(data) =>
       <div>
         <h2>
           "{data##createPost
            ->Belt.Option.map(post =>
                \"Post#\" ++ post##id ++ \" \" ++ post##title
              )
            ->Belt.Option.getWithDefault(\"\")
            ->React.string}"
           ->React.string
         </h2>
       </div>
     | _ =>
       <form
         onSubmit={event => {
           ReactEvent.Synthetic.preventDefault(event);
           reform.submit();
         }}>
         <FieldString field=BotForm.FormConfig.Name label="Name" />
         <FieldString
           field=BotForm.FormConfig.Description
           label="Description"
         />
         {reform.state.formState == Submitting
            ? <p> {React.string("Saving...")} </p>
            : <button type_="submit"> {"Submit" |> React.string} </button>}
       </form>
     }}
  </BotForm.Form.Provider>;
};