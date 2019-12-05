[@bs.config {jsx: 3}];

module CMutationForm =
  MutationForm.Make({
    type queryType = BotUpdate.Query.t;
  });

[@react.component]
let make = (~pageParams: Types.pageParams, ~loggedUser: Types.user) => {
  let getForm = (bot: option(Types.bot)) =>
    <BotUpdate.Mutation>
      ...{(mutation, data) => {
        let onSubmit = (values: BotForm.Form.onSubmitAPI) => {
          BotUpdate.mutate(
            mutation,
            pageParams.id,
            values.state.values.name,
            values.state.values.description,
            values.state.values.competitorType,
            values.state.values.picture,
          );
          None;
        };

        BotForm.withForm(
          onSubmit,
          bot,
          ({handleChange, state, submit}) => {
            let form = BotForm.formFields(state, handleChange, () => submit());

            CMutationForm.showWithLoading2(
              ~result=data.result,
              ~form,
              ~onSuccess=_ => BotForm.onSuccess(loggedUser, ()),
              (),
            );
          },
        );
      }}
    </BotUpdate.Mutation>;

  let botId = pageParams.id;
  let body = BotGet.component(~id=botId, getForm);

  <SLayout head={SLayout.Header.textDiv("Edit a Bot")}> body </SLayout>;
};