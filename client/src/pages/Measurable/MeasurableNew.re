open Utils;
open Rationale;
open Rationale.Option;
open Result.Infix;
open Rationale.Function.Infix;

let ste = ReasonReact.string;

module CreateMeasurable = [%graphql
  {|
             mutation createMeasurable($name: String!, $valueType:valueType!) {
                 createMeasurable(name: $name, valueType: $valueType) {
                   id
                 }
             }
     |}
];

module CreateMeasurableMutation =
  ReasonApollo.CreateMutation(CreateMeasurable);

module SignUpParams = {
  type state = {name: string};
  type fields = [ | `name];
  let lens = [(`name, s => s.name, (_s, name) => {name: name})];
};

module SignUpForm = ReForm.Create(SignUpParams);

let component = ReasonReact.statelessComponent("Measurables");
let make = _children => {
  ...component,
  render: self =>
    CreateMeasurableMutation.make((mutation, _) =>
      SignUpForm.make(
        ~onSubmit=e => e |> Js.log |> ignore,
        ~initialState={name: ""},
        ~schema=[(`name, Custom(_ => None))],
        ({handleSubmit, handleChange, form, _}) =>
          <form onSubmit=(ReForm.Helpers.handleDomFormSubmit(handleSubmit))>
            <label>
              <input
                value=form.values.name
                onChange=(
                  ReForm.Helpers.handleDomFormChange(handleChange(`name))
                )
              />
            </label>
            <button> ("Submit" |> ste) </button>
          </form>,
      )
      |> ReasonReact.element
    )
    |> ReasonReact.element,
};

/* CreateMeasurableMutation.make((mutation, _) =>
     SignUpForm.make(_ => <div />) |> ReasonReact.element
   )
   |> ReasonReact.element, */
/* <SignUpForm
     onSubmit=(
       ({values}) => {
         let mut =
           CreateMeasurable.make(~name=values.name, ~valueType=`FLOAT, ());
         mutation(
           ~variables=mut##variables,
           ~refetchQueries=[|"getMeasurable"|],
           (),
         )
         |> ignore;
       }
     )
     initialState={name: ""}
     schema=[(`name, Custom(e => None))]>
     ...(
          ({handleSubmit, handleChange, form, _}) =>
            <form
              onSubmit=(ReForm.Helpers.handleDomFormSubmit(handleSubmit))>
              <label>
                <input
                  value=form.values.name
                  onChange=(
                    ReForm.Helpers.handleDomFormChange(
                      handleChange(`name),
                    )
                  )
                />
              </label>
              <button> ("Submit" |> ste) </button>
            </form>
        )
   </SignUpForm> */